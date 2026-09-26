#pragma once

#include <QPointer>
#include <QTimer>
#include <QWidget>

#include <Gris/Algo/Types.hpp>

#include <optional>
#include <vector>

namespace score
{
struct DocumentContext;
}

namespace Gris
{
class SpatModel;

class SpatView final : public QWidget
{
public:
  SpatView(SpatModel& proc, const score::DocumentContext& ctx, QWidget* parent);
  ~SpatView() override;

  QSize sizeHint() const override;

private:
  struct Projected
  {
    QPointF point;
    float depth{};
  };

  void paintEvent(QPaintEvent*) override;
  void mousePressEvent(QMouseEvent*) override;
  void mouseMoveEvent(QMouseEvent*) override;
  void wheelEvent(QWheelEvent*) override;

  [[nodiscard]] std::optional<Projected>
  project(CartesianVector const& p) const noexcept;

  QPointer<SpatModel> m_proc;
  QTimer m_refresh;
  QPointF m_lastMouse;
  float m_azimuth{30.f};
  float m_elevation{25.f};
  float m_distance{4.f};
  std::vector<float> m_levels;
};
}
