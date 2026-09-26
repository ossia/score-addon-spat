#pragma once

#include <Effect/EffectFactory.hpp>

#include <score/graphics/RectItem.hpp>

#include <Gris/Metadata.hpp>

#include <vector>

namespace score
{
class GraphicsLayout;
class GraphicsIORootLayout;
class GraphicsDefaultOutletLayout;
}

namespace Gris
{
class SpatModel;

class SpatItem final : public score::EmptyRectItem
{
public:
  SpatItem(const SpatModel& proc, const Process::Context& ctx, QGraphicsItem* parent);
  ~SpatItem() override;

private:
  void reset();
  void recreate();
  void relayout();
  void createPager(int pageCount);
  void setPage(int page);

  const SpatModel& m_proc;
  const Process::Context& m_ctx;
  score::GraphicsIORootLayout* m_root{};
  score::GraphicsLayout* m_table{};
  score::GraphicsDefaultOutletLayout* m_outlets{};
  std::vector<score::GraphicsLayout*> m_stacks;
  score::EmptyRectItem* m_pager{};
  int m_page{};
  bool m_needRecreate{};
};

class LayerFactory final : public Process::EffectLayerFactory_Base
{
  UuidKey<Process::ProcessModel> concreteKey() const noexcept override;
  bool matches(const UuidKey<Process::ProcessModel>& p) const override;
  score::ResizeableItem* makeItem(
      const Process::ProcessModel& proc, const Process::Context& ctx,
      QGraphicsItem* parent) const override;
  bool hasExternalUI(
      const Process::ProcessModel& proc,
      const score::DocumentContext& ctx) const noexcept override;
  QWidget* makeExternalUI(
      Process::ProcessModel& proc, const score::DocumentContext& ctx,
      QWidget* parent) const override;
};
}
