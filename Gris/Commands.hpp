#pragma once

#include <score/command/Command.hpp>
#include <score/command/PropertyCommand.hpp>
#include <score/model/path/Path.hpp>
#include <score/model/path/PathSerialization.hpp>

#include <Gris/Model.hpp>

namespace Gris
{
const CommandGroupKey& CommandFactoryName();

class SetSourceCount final : public score::PropertyCommand
{
  SCORE_COMMAND_DECL(Gris::CommandFactoryName(), SetSourceCount, "Set source count")
public:
  SetSourceCount(const SpatModel& model, int newCount)
      : score::PropertyCommand{model, "sourceCount", QVariant::fromValue(newCount)}
  {
  }
};
}
