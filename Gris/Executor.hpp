#pragma once

#include <Process/Execution/ProcessComponent.hpp>

#include <ossia/dataflow/node_process.hpp>
#include <ossia/dataflow/port.hpp>

#include <Gris/LayoutCache.hpp>
#include <Gris/Model.hpp>

#include <string>
#include <vector>

namespace Gris
{
class SpatNode;

class Executor final
    : public Execution::ProcessComponent_T<SpatModel, ossia::node_process>
{
  COMPONENT_METADATA("b5c0e91a-3d47-4f8a-9c21-6ae0d4f37b52")
public:
  Executor(SpatModel& proc, const Execution::Context& ctx, QObject* parent);
  ~Executor() override;

private:
  void useSetup(std::string key, SpeakerSetup setup);
  void sendLayout(LayoutPtr layout);
  void recomputePorts();
  void connectControls();
  void pushControlValue(ossia::value_inlet* port, ossia::value v);
  void applyPendingSpeakerList();

  Process::Inlets m_oldInlets;
  Process::Outlets m_oldOutlets;
  std::vector<QMetaObject::Connection> m_controlConnections;
  std::string m_layoutKey;
};

using ExecutorFactory = Execution::ProcessComponentFactory_T<Executor>;
}
