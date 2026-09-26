#pragma once

#include <QObject>

#include <Gris/Algo/Spatializer.hpp>

#include <functional>
#include <memory>
#include <string>

namespace Gris
{
using LayoutPtr = std::shared_ptr<Layout const>;

[[nodiscard]] LayoutPtr findLayout(std::string const& key);

void requestLayout(
    std::string const& key, SpeakerSetup setup, QObject* context,
    std::function<void(LayoutPtr)> done);
}
