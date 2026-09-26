#pragma once

#include <QString>

#include <Gris/Algo/SpeakerSetup.hpp>

#include <optional>

namespace Gris
{
enum class SpeakerSetupFormat
{
  unknown,
  legacy,
  intermediate,
  valueTree
};

struct SpeakerSetupReadResult
{
  std::optional<SpeakerSetup> setup{};
  SpeakerSetupFormat format{SpeakerSetupFormat::unknown};
  QString error{};

  [[nodiscard]] explicit operator bool() const noexcept { return setup.has_value(); }
};

[[nodiscard]] SpeakerSetupReadResult readSpeakerSetup(QByteArray const& xml);

[[nodiscard]] SpeakerSetupReadResult readSpeakerSetupFile(QString const& path);

[[nodiscard]] QByteArray writeSpeakerSetup(SpeakerSetup const& setup);

[[nodiscard]] QString
writeSpeakerSetupFile(SpeakerSetup const& setup, QString const& path);

}
