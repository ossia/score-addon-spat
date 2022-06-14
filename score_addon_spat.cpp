#include "score_addon_spat.hpp"
<<<<<<< HEAD
=======

>>>>>>> 9cd6993ddf1fab629d4bb9b1576a20b219992c62
#include <Spat/StereoToMono.hpp>
#include <Spat/StereoPanning.hpp>
#include <Spat/Rotator.hpp>
#include <Spat/AmbiToBinaural.hpp>
#include <Spat/MonoToAmbi.hpp>
#include <Spat/Vbap.hpp>

#include <Spat/Example.hpp>
#include <Spat/Spatatouille.hpp>

#include <Avnd/Factories.hpp>
#include <score/plugins/FactorySetup.hpp>
#include <score_plugin_engine.hpp>

/**
 * This file instantiates the classes that are provided by this plug-in.
 */
score_addon_spat::score_addon_spat() = default;
score_addon_spat::~score_addon_spat() = default;

std::vector<std::unique_ptr<score::InterfaceBase>>
score_addon_spat::factories(
    const score::ApplicationContext& ctx,
    const score::InterfaceKey& key) const
{
<<<<<<< HEAD
  return Avnd::instantiate_fx<
            Spat::StereoToMono
          , Spat::StereoPanning
          , Spat::Rotator
          , Spat::AmbiToBinaural
          , Spat::MonoToAmbi
          , Spat::Vbap
            >(ctx, key);
=======
  return Avnd::instantiate_fx<   
            Spat::Spatatouille
//          , Spat::StereoToMono
//          , Spat::StereoPanning
//          , Spat::Rotator
//          , Spat::AmbiToBinaural
//          , Spat::Example
          >(ctx, key);
>>>>>>> 9cd6993ddf1fab629d4bb9b1576a20b219992c62
}

std::vector<score::PluginKey> score_addon_spat::required() const
{
  return {score_plugin_engine::static_key()};
}

#include <score/plugins/PluginInstances.hpp>
SCORE_EXPORT_PLUGIN(score_addon_spat)
