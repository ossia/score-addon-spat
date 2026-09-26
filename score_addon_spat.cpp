#include "score_addon_spat.hpp"

#include <Gris/Commands.hpp>
#include <Gris/Executor.hpp>
#include <Gris/Inspector.hpp>
#include <Gris/Layer.hpp>
#include <Gris/Model.hpp>
#include <Gris/SpeakerSetupInlet.hpp>

#include <Spat/AmbiToBinaural.hpp>
#include <Spat/MonoToAmbi.hpp>
#include <Spat/Rotator.hpp>
#include <Spat/Spatatouille.hpp>
#include <Spat/StereoPanning.hpp>
#include <Spat/StereoToMono.hpp>

#include <Avnd/Factories.hpp>
#include <Dataflow/WidgetInletFactory.hpp>
#include <Effect/EffectFactory.hpp>
#include <Process/Dataflow/PortFactory.hpp>
#include <Process/GenericProcessFactory.hpp>

#include <score/plugins/FactorySetup.hpp>

#include <score_addon_spat_commands_files.hpp>
#include <score_plugin_engine.hpp>

score_addon_spat::score_addon_spat() = default;
score_addon_spat::~score_addon_spat() = default;

std::vector<score::InterfaceBase*> score_addon_spat::factories(
    const score::ApplicationContext& ctx, const score::InterfaceKey& key) const
{
  auto fx = instantiate_factories<
      score::ApplicationContext,
      FW<Process::ProcessModelFactory, Process::ProcessFactory_T<Gris::SpatModel>>,
      FW<Process::LayerFactory, Gris::LayerFactory>,
      FW<Process::PortFactory,
         Dataflow::WidgetInletFactory<
             Gris::SpeakerSetupInlet, WidgetFactory::SpeakerSetupWidget>>,
      FW<Execution::ProcessComponentFactory, Gris::ExecutorFactory>,
      FW<Inspector::InspectorWidgetFactory, Gris::InspectorFactory>>(ctx, key);

  Avnd::instantiate_fx<Spat::Spatatouille>(fx, ctx, key);
  Avnd::instantiate_fx<Spat::StereoToMono>(fx, ctx, key);
  Avnd::instantiate_fx<Spat::StereoPanning>(fx, ctx, key);
  Avnd::instantiate_fx<Spat::Rotator>(fx, ctx, key);
  Avnd::instantiate_fx<Spat::AmbiToBinaural>(fx, ctx, key);
  Avnd::instantiate_fx<Spat::MonoToAmbi>(fx, ctx, key);
  return fx;
}

std::pair<const CommandGroupKey, CommandGeneratorMap> score_addon_spat::make_commands()
{
  using namespace Gris;
  std::pair<const CommandGroupKey, CommandGeneratorMap> cmds{
      CommandFactoryName(), CommandGeneratorMap{}};

  ossia::for_each_type<
#include <score_addon_spat_commands.hpp>
      >(score::commands::FactoryInserter{cmds.second});

  return cmds;
}

std::vector<score::PluginKey> score_addon_spat::required() const
{
  return {score_plugin_engine::static_key()};
}

#include <score/plugins/PluginInstances.hpp>
SCORE_EXPORT_PLUGIN(score_addon_spat)
