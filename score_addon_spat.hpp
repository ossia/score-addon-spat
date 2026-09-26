#pragma once
#include <score/command/Command.hpp>
#include <score/plugins/Interface.hpp>
#include <score/plugins/qt_interfaces/CommandFactory_QtInterface.hpp>
#include <score/plugins/qt_interfaces/FactoryInterface_QtInterface.hpp>
#include <score/plugins/qt_interfaces/PluginRequirements_QtInterface.hpp>

#include <score_addon_spat_export.h>

#include <utility>
#include <vector>

class SCORE_ADDON_SPAT_EXPORT score_addon_spat final
    : public score::Plugin_QtInterface
    , public score::FactoryInterface_QtInterface
    , public score::CommandFactory_QtInterface
{
  SCORE_PLUGIN_METADATA(1, "24a8f6cc-73ed-4b57-ad3e-b046f00fa1a0")
public:
  score_addon_spat();
  ~score_addon_spat() override;

private:
  std::vector<score::InterfaceBase*> factories(
      const score::ApplicationContext& ctx,
      const score::InterfaceKey& key) const override;

  std::pair<const CommandGroupKey, CommandGeneratorMap> make_commands() override;

  std::vector<score::PluginKey> required() const override;
};
