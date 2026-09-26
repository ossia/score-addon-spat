#pragma once
#include <Process/ProcessMetadata.hpp>

#include <score/plugins/UuidKey.hpp>

#include <QString>

namespace Gris
{
class SpatModel;
}

PROCESS_METADATA(
    , Gris::SpatModel, "3bbf1f2a-64b5-4c58-8a4e-2b0f3a19cd71", "GrisSpat",
    "Spatialization (VBAP / MBAP)", Process::ProcessCategory::AudioEffect,
    "Audio/Spatialization",
    "Spatializes mono sources over a speaker setup with the GRIS VBAP (dome) and "
    "MBAP (cube) algorithms from SpatGRIS. One audio inlet per source; each source "
    "picks its algorithm independently, at realtime.",
    "GRIS / SAT", (QStringList{"Spatialization", "VBAP", "MBAP", "SpatGRIS", "GRIS"}),
    {}, {}, QUrl{},
    Process::ProcessFlags::SupportsAll | Process::ProcessFlags::DynamicPorts)
