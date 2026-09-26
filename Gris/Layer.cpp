#include <Process/Dataflow/Port.hpp>
#include <Process/Dataflow/PortFactory.hpp>
#include <Process/Dataflow/PortItem.hpp>
#include <Process/Dataflow/PortType.hpp>
#include <Process/ProcessContext.hpp>

#include <Control/Layout.hpp>

#include <score/application/GUIApplicationContext.hpp>
#include <score/graphics/GraphicsItem.hpp>
#include <score/graphics/TextItem.hpp>
#include <score/graphics/layouts/GraphicsBoxLayout.hpp>
#include <score/graphics/layouts/GraphicsGridLayout.hpp>
#include <score/graphics/widgets/QGraphicsPixmapButton.hpp>
#include <score/widgets/Pixmap.hpp>

#include <ossia-qt/invoke.hpp>

#include <Gris/Layer.hpp>
#include <Gris/Model.hpp>
#include <Gris/SpatView.hpp>

#include <cmath>

#include <algorithm>
#include <initializer_list>
#include <vector>

namespace Gris
{
namespace
{
constexpr qreal cellSpacing = 4.;

class PortTableLayout final : public score::GraphicsLayout
{
public:
  PortTableLayout(int columns, QGraphicsItem* parent)
      : score::GraphicsLayout{parent}
      , m_columns{columns}
  {
  }

  void layout() override
  {
    const auto items = childItems();
    updateChildrenRects(items);

    auto const count = int(items.size());
    std::vector<qreal> widths(std::size_t(m_columns), 0.);
    std::vector<qreal> heights(std::size_t((count + m_columns - 1) / m_columns), 0.);
    for(int i = 0; i < count; ++i)
    {
      auto const r = items[i]->boundingRect();
      auto& w = widths[std::size_t(i % m_columns)];
      auto& h = heights[std::size_t(i / m_columns)];
      w = std::max(w, r.width());
      h = std::max(h, r.height());
    }

    qreal y = 0.;
    for(std::size_t row = 0; row < heights.size(); ++row)
    {
      qreal x = 0.;
      for(std::size_t col = 0; col < widths.size(); ++col)
      {
        auto const i = int(row) * m_columns + int(col);
        if(i >= count)
          break;
        auto* item = items[i];
        auto const h = item->boundingRect().height();
        item->setPos(x, y + (heights[row] - h) / 2.);
        x += widths[col] + cellSpacing;
      }
      y += heights[row] + cellSpacing;
    }
  }

private:
  int m_columns{};
};

void deletePortItems(QGraphicsItem* it)
{
  const auto items = it->childItems();
  for(auto* ptr : items)
  {
    if(auto* port = qgraphicsitem_cast<Dataflow::PortItem*>(ptr))
      deleteGraphicsItem(port);
    else
      deletePortItems(ptr);
  }
}

constexpr int tableColumns = 3;
constexpr qreal stackSpacing = 2.;
constexpr int sourcesPerPage = 4;
constexpr qreal pagerHeight = 13.;
constexpr qreal pagerSpacing = 3.;
}

SpatItem::SpatItem(
    const SpatModel& proc, const Process::Context& ctx, QGraphicsItem* parent)
    : score::EmptyRectItem{parent}
    , m_proc{proc}
    , m_ctx{ctx}
{
  connect(&proc, &Process::ProcessModel::inletsChanged, this, &SpatItem::reset);
  connect(&proc, &Process::ProcessModel::outletsChanged, this, &SpatItem::reset);
  connect(this, &score::ResizeableItem::childrenSizeChanged, this, &SpatItem::relayout);
  connect(this, &score::ResizeableItem::minimumWidthChanged, this, &SpatItem::relayout);
  reset();
}

SpatItem::~SpatItem() = default;

void SpatItem::reset()
{
  if(m_root)
  {
    deletePortItems(m_root);
    m_root->setVisible(false);
    m_root->deleteLater();
    m_root = nullptr;
    m_table = nullptr;
    m_outlets = nullptr;
    m_stacks.clear();
  }

  delete m_pager;
  m_pager = nullptr;

  m_needRecreate = true;
  ossia::qt::run_async(this, &SpatItem::recreate);
}

void SpatItem::recreate()
{
  if(!m_needRecreate)
    return;
  m_needRecreate = false;

  auto& portFactory = m_ctx.app.interfaces<Process::PortFactoryList>();

  m_root = new score::GraphicsIORootLayout{this};
  m_table = new PortTableLayout{tableColumns, m_root};
  m_outlets = new score::GraphicsDefaultOutletLayout{m_root};

  Process::LayoutBuilderBase b{*this,           m_proc,           m_ctx,   portFactory,
                               m_proc.inlets(), m_proc.outlets(), m_table, {}};

  auto const& inlets = m_proc.inlets();
  auto place = [&](int index, QGraphicsItem* parent) {
    auto* inlet = inlets[std::size_t(index)];
    b.layout = parent;
    auto item = b.makePort(*inlet);
    if(item.container)
      item.container->setParentItem(parent);
    if(auto* control = qobject_cast<Process::ControlInlet*>(inlet))
      connect(
          control, &Process::ControlInlet::domainChanged, this, &SpatItem::reset,
          Qt::UniqueConnection);
  };
  auto stack = [&](std::initializer_list<int> indices) {
    auto* column = new score::GraphicsVBoxLayout{m_table};
    column->setPadding(stackSpacing);
    m_stacks.push_back(column);
    for(int index : indices)
      place(index, column);
  };

  stack({SpatModel::SpeakerList, SpatModel::SpeakerSetupPort});
  place(SpatModel::Interpolation, m_table);
  stack({SpatModel::AttenuationCutoff, SpatModel::AttenuationGain});

  auto const sources = m_proc.sourceCount();
  auto const pageCount = std::max(1, (sources + sourcesPerPage - 1) / sourcesPerPage);
  m_page = std::clamp(m_page, 0, pageCount - 1);
  auto const firstSource = m_page * sourcesPerPage;
  auto const lastSource = std::min(sources, firstSource + sourcesPerPage);
  for(int source = firstSource; source < lastSource; ++source)
  {
    auto const first = SpatModel::firstInletOf(source);
    place(first + SpatModel::Audio, m_table);
    place(first + SpatModel::Position, m_table);
    stack(
        {first + SpatModel::AzimuthSpan, first + SpatModel::ZenithSpan,
         first + SpatModel::Mode});
  }

  b.layout = m_outlets;
  for(auto* outlet : m_proc.outlets())
  {
    auto item = b.makePort(*outlet);
    if(item.container)
      item.container->setParentItem(m_outlets);
  }

  if(pageCount > 1)
    createPager(pageCount);

  relayout();
}

void SpatItem::createPager(int pageCount)
{
  static const auto prevOn = score::get_pixmap(":/icons/arrow_left_on.png");
  static const auto prevOff = score::get_pixmap(":/icons/arrow_left_disabled.png");
  static const auto nextOn = score::get_pixmap(":/icons/arrow_right_on.png");
  static const auto nextOff = score::get_pixmap(":/icons/arrow_right_disabled.png");

  m_pager = new score::EmptyRectItem{this};
  auto* prev = new score::QGraphicsPixmapButton{prevOn, prevOff, m_pager};
  auto* next = new score::QGraphicsPixmapButton{nextOn, nextOff, m_pager};
  auto* label = new score::SimpleTextItem{Process::labelBrush().main, m_pager};
  label->setText(QStringLiteral("%1/%2").arg(m_page + 1).arg(pageCount));

  auto const labelWidth = label->boundingRect().width();
  auto const arrowWidth = prev->boundingRect().width();
  auto const center = [](qreal h) { return std::round((pagerHeight - h) / 2.); };
  prev->setPos(0., center(prev->boundingRect().height()));
  label->setPos(arrowWidth + pagerSpacing, center(label->boundingRect().height()));
  next->setPos(
      arrowWidth + 2. * pagerSpacing + labelWidth,
      center(next->boundingRect().height()));
  m_pager->setRect(
      {0., 0., 2. * arrowWidth + 2. * pagerSpacing + labelWidth + 2., pagerHeight});

  connect(prev, &score::QGraphicsPixmapButton::clicked, this, [this] {
    ossia::qt::run_async(this, [this] { setPage(m_page - 1); });
  });
  connect(next, &score::QGraphicsPixmapButton::clicked, this, [this] {
    ossia::qt::run_async(this, [this] { setPage(m_page + 1); });
  });
}

void SpatItem::setPage(int page)
{
  auto const pageCount
      = std::max(1, (m_proc.sourceCount() + sourcesPerPage - 1) / sourcesPerPage);
  page = std::clamp(page, 0, pageCount - 1);
  if(page == m_page)
    return;
  m_page = page;
  reset();
}

void SpatItem::relayout()
{
  if(!m_root)
    return;

  for(auto* column : m_stacks)
  {
    column->layout();
    column->fitChildrenRect();
  }
  m_table->layout();
  m_table->fitChildrenRect();
  m_outlets->layout();
  m_outlets->fitChildrenRect();

  m_root->setMinimumWidth(minimumWidth());
  m_root->layout();
  m_root->setRect(m_root->childrenBoundingRect());
  m_root->fitChildrenRect();

  auto r = m_root->rect();
  if(m_pager)
  {
    m_pager->setPos(0., r.height());
    r.setHeight(r.height() + m_pager->rect().height());
    r.setWidth(std::max(r.width(), m_pager->rect().width()));
  }
  setRect(r);
}

UuidKey<Process::ProcessModel> LayerFactory::concreteKey() const noexcept
{
  return Metadata<ConcreteKey_k, SpatModel>::get();
}

bool LayerFactory::matches(const UuidKey<Process::ProcessModel>& p) const
{
  return p == Metadata<ConcreteKey_k, SpatModel>::get();
}

score::ResizeableItem* LayerFactory::makeItem(
    const Process::ProcessModel& proc, const Process::Context& ctx,
    QGraphicsItem* parent) const
{
  return new SpatItem{safe_cast<const SpatModel&>(proc), ctx, parent};
}

bool LayerFactory::hasExternalUI(
    const Process::ProcessModel&, const score::DocumentContext&) const noexcept
{
  return true;
}

QWidget* LayerFactory::makeExternalUI(
    Process::ProcessModel& proc, const score::DocumentContext& ctx,
    QWidget* parent) const
{
  return new SpatView{safe_cast<SpatModel&>(proc), ctx, parent};
}
}
