#include <Process/Dataflow/Port.hpp>

#include <ossia/network/value/value_conversion.hpp>

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QWheelEvent>

#include <Gris/LiveSources.hpp>
#include <Gris/Model.hpp>
#include <Gris/SpatView.hpp>

#include <cmath>

#include <algorithm>
#include <numbers>
#include <vector>

namespace Gris
{
namespace
{
constexpr float degToRad = std::numbers::pi_v<float> / 180.f;

struct Vec
{
  float x{}, y{}, z{};
};

Vec operator-(Vec a, Vec b) noexcept
{
  return {a.x - b.x, a.y - b.y, a.z - b.z};
}

float dot(Vec a, Vec b) noexcept
{
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec cross(Vec a, Vec b) noexcept
{
  return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

Vec normalized(Vec a) noexcept
{
  auto const n = std::sqrt(dot(a, a));
  return n > 0.f ? Vec{a.x / n, a.y / n, a.z / n} : a;
}

QColor speakerColor(float level)
{
  auto const db = 20.f * std::log10(std::max(level, 1e-6f));
  auto const t = std::clamp((db + 60.f) / 60.f, 0.f, 1.f);
  auto mix = [t](int from, int to) { return int(std::lround(from + (to - from) * t)); };
  return QColor{mix(105, 255), mix(107, 238), mix(114, 160)};
}

QColor sourceColor(int index)
{
  return QColor::fromHsvF(std::fmod(0.07 + index * 0.618034, 1.0), 0.65, 1.0);
}

struct SourceDraw
{
  CartesianVector position;
  bool mbap{};
  int index{};
};

struct Marker
{
  enum Kind
  {
    Speaker,
    Source
  } kind{};
  QPointF point;
  float depth{};
  int number{};
  bool square{};
  QColor color;
};
}

SpatView::SpatView(SpatModel& proc, const score::DocumentContext&, QWidget* parent)
    : QWidget{parent}
    , m_proc{&proc}
{
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle(tr("%1 - speakers").arg(proc.metadata().getName()));
  setMinimumSize(240, 240);
  setMouseTracking(false);

  proc.externalUI = this;
  proc.externalUIVisible(true);

  m_refresh.setInterval(33);
  connect(&m_refresh, &QTimer::timeout, this, qOverload<>(&QWidget::update));
  m_refresh.start();
}

SpatView::~SpatView()
{
  if(m_proc)
  {
    m_proc->externalUI = nullptr;
    m_proc->externalUIVisible(false);
  }
}

QSize SpatView::sizeHint() const
{
  return {520, 480};
}

std::optional<SpatView::Projected>
SpatView::project(CartesianVector const& p) const noexcept
{
  auto const az = m_azimuth * degToRad;
  auto const el = m_elevation * degToRad;
  Vec const eye{
      m_distance * std::cos(el) * std::sin(az),
      -m_distance * std::cos(el) * std::cos(az), m_distance * std::sin(el)};
  auto const forward = normalized(Vec{} - eye);
  auto const right = normalized(cross(forward, Vec{0.f, 0.f, 1.f}));
  auto const up = cross(right, forward);

  auto const v = Vec{p.x, p.y, p.z} - eye;
  auto const depth = dot(v, forward);
  if(depth < 0.05f)
    return std::nullopt;

  auto const focal = 0.9f * float(std::min(width(), height()));
  return Projected{
      QPointF{
          width() / 2. + focal * dot(v, right) / depth,
          height() / 2. - focal * dot(v, up) / depth},
      depth};
}

void SpatView::paintEvent(QPaintEvent*)
{
  QPainter painter{this};
  painter.setRenderHint(QPainter::Antialiasing);
  painter.fillRect(rect(), QColor{24, 26, 30});

  if(!m_proc)
    return;

  auto const layout = m_proc->layout();
  bool const cube = layout && layout->setup.spatMode == SpatMode::mbap;

  auto line = [&](CartesianVector a, CartesianVector b) {
    auto const pa = project(a);
    auto const pb = project(b);
    if(pa && pb)
      painter.drawLine(pa->point, pb->point);
  };

  painter.setPen(QPen{QColor{70, 76, 86}, 1.});
  if(cube)
  {
    for(float a : {-1.f, 1.f})
      for(float b : {-1.f, 1.f})
      {
        line({-1.f, a, b}, {1.f, a, b});
        line({a, -1.f, b}, {a, 1.f, b});
        line({a, b, -1.f}, {a, b, 1.f});
      }
  }
  else
  {
    constexpr int steps = 48;
    for(float elevation : {0.f, 30.f, 60.f})
    {
      auto const r = std::cos(elevation * degToRad);
      auto const z = std::sin(elevation * degToRad);
      for(int i = 0; i < steps; ++i)
      {
        auto const a0 = 2.f * std::numbers::pi_v<float> * float(i) / steps;
        auto const a1 = 2.f * std::numbers::pi_v<float> * float(i + 1) / steps;
        line(
            {r * std::cos(a0), r * std::sin(a0), z},
            {r * std::cos(a1), r * std::sin(a1), z});
      }
    }
    for(int m = 0; m < 8; ++m)
    {
      auto const a = 2.f * std::numbers::pi_v<float> * float(m) / 8.f;
      for(int i = 0; i < 12; ++i)
      {
        auto const e0 = float(i) / 12.f * std::numbers::pi_v<float> / 2.f;
        auto const e1 = float(i + 1) / 12.f * std::numbers::pi_v<float> / 2.f;
        line(
            {std::cos(e0) * std::cos(a), std::cos(e0) * std::sin(a), std::sin(e0)},
            {std::cos(e1) * std::cos(a), std::cos(e1) * std::sin(a), std::sin(e1)});
      }
    }
  }

  painter.setPen(QPen{QColor{150, 160, 175}, 1.5});
  line({0.f, 0.f, 0.f}, {0.f, 0.3f, 0.f});

  auto const& live = m_proc->liveSources();
  bool const running = m_proc->executing();
  m_levels.resize(live->levels.size(), 0.f);
  for(std::size_t c = 0; c < m_levels.size(); ++c)
  {
    auto const level = running ? live->levels[c].load(std::memory_order_relaxed) : 0.f;
    m_levels[c] = std::max(level, m_levels[c] * 0.8f);
  }

  std::vector<Marker> markers;
  if(layout)
  {
    for(std::size_t i = 0; i < layout->flat.size(); ++i)
    {
      auto const& speaker = layout->flat[i];
      auto const channel = layout->channelOfSpeaker[i];
      auto const level = channel >= 0 && std::size_t(channel) < m_levels.size()
                             ? m_levels[std::size_t(channel)]
                             : 0.f;
      if(auto p = project(speaker.data.position.getCartesian()))
        markers.push_back(
            {Marker::Speaker, p->point, p->depth, speaker.patch.get(), true,
             speaker.data.isDirectOutOnly ? QColor{70, 70, 72} : speakerColor(level)});
    }
  }

  auto const& inlets = m_proc->inlets();
  for(int source = 0; source < m_proc->sourceCount(); ++source)
  {
    auto const first = std::size_t(SpatModel::firstInletOf(source));
    CartesianVector position{};
    bool mbap{};
    auto const& l = live->sources[std::size_t(source)];
    if(running && l.placed.load(std::memory_order_relaxed))
    {
      position
          = {l.x.load(std::memory_order_relaxed), l.y.load(std::memory_order_relaxed),
             l.z.load(std::memory_order_relaxed)};
      mbap = l.mbap.load(std::memory_order_relaxed);
    }
    else
    {
      if(auto* pos
         = qobject_cast<Process::ControlInlet*>(inlets[first + SpatModel::Position]))
      {
        auto const v = ossia::convert<ossia::vec3f>(pos->value());
        position = {v[0], v[1], v[2]};
      }
      if(auto* mode
         = qobject_cast<Process::ControlInlet*>(inlets[first + SpatModel::Mode]))
        mbap = ossia::convert<std::string>(mode->value()).find("Cube")
               != std::string::npos;
    }

    if(auto p = project(position))
      markers.push_back(
          {Marker::Source, p->point, p->depth, source + 1, mbap, sourceColor(source)});
  }

  std::sort(markers.begin(), markers.end(), [](auto const& a, auto const& b) {
    return a.depth > b.depth;
  });

  QFont small = font();
  small.setPointSizeF(7.5);
  QFont bold = font();
  bold.setPointSizeF(8.5);
  bold.setBold(true);

  for(auto const& m : markers)
  {
    auto const scale = std::clamp(3.f / m.depth, 0.5f, 2.f);
    if(m.kind == Marker::Speaker)
    {
      auto const s = 4. * scale;
      painter.setPen(QPen{QColor{30, 30, 34}, 1.});
      painter.setBrush(m.color);
      painter.drawRect(QRectF{m.point.x() - s, m.point.y() - s, 2 * s, 2 * s});
      painter.setFont(small);
      painter.setPen(QColor{140, 145, 155});
      painter.drawText(m.point + QPointF{s + 2., -s}, QString::number(m.number));
    }
    else
    {
      auto const s = 7. * scale;
      painter.setPen(QPen{m.color.darker(160), 1.5});
      painter.setBrush(m.color);
      QRectF const r{m.point.x() - s, m.point.y() - s, 2 * s, 2 * s};
      if(m.square)
        painter.drawRect(r);
      else
        painter.drawEllipse(r);
      painter.setFont(bold);
      painter.setPen(QColor{20, 20, 20});
      painter.drawText(r, Qt::AlignCenter, QString::number(m.number));
    }
  }

  painter.setFont(small);
  painter.setPen(QColor{120, 126, 136});
  painter.drawText(
      rect().adjusted(8, 6, -8, -6), Qt::AlignLeft | Qt::AlignBottom,
      tr("drag to orbit, wheel to zoom   %1   circle: VBAP, square: MBAP")
          .arg(
              layout ? tr("%n speakers", nullptr, int(layout->flat.size()))
                     : tr("speaker layout not ready")));
}

void SpatView::mousePressEvent(QMouseEvent* event)
{
  m_lastMouse = event->position();
}

void SpatView::mouseMoveEvent(QMouseEvent* event)
{
  auto const delta = event->position() - m_lastMouse;
  m_lastMouse = event->position();
  m_azimuth -= float(delta.x()) * 0.5f;
  m_elevation = std::clamp(m_elevation + float(delta.y()) * 0.5f, -89.f, 89.f);
  update();
}

void SpatView::wheelEvent(QWheelEvent* event)
{
  auto const steps = float(event->angleDelta().y()) / 120.f;
  m_distance = std::clamp(m_distance * std::pow(0.9f, steps), 1.5f, 12.f);
  update();
}
}
