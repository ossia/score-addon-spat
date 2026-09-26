#pragma once

#include <Process/Dataflow/Port.hpp>
#include <Process/Dataflow/PortItem.hpp>

#include <score/serialization/VisitorCommon.hpp>

#include <Gris/Algo/SpeakerSetup.hpp>

#include <score_addon_spat_export.h>

namespace Gris
{
struct SpeakerSetupInlet;
}

UUID_METADATA(
    SCORE_ADDON_SPAT_EXPORT, Process::Port, Gris::SpeakerSetupInlet,
    "5a7f6c24-9d3b-41f0-8e6a-7c2d5b9e1f48")

namespace Gris
{
struct SCORE_ADDON_SPAT_EXPORT SpeakerSetupInlet : public Process::ControlInlet
{
  MODEL_METADATA_IMPL(SpeakerSetupInlet)
  SpeakerSetupInlet(const QString& name, Id<Process::Port> id, QObject* parent);
  ~SpeakerSetupInlet();

  SpeakerSetupInlet(DataStream::Deserializer& vis, QObject* parent);
  SpeakerSetupInlet(JSONObject::Deserializer& vis, QObject* parent);
  SpeakerSetupInlet(DataStream::Deserializer&& vis, QObject* parent);
  SpeakerSetupInlet(JSONObject::Deserializer&& vis, QObject* parent);

  [[nodiscard]] SpeakerSetup setup() const noexcept;
  void setSetup(SpeakerSetup const& setup);

  using Process::ControlInlet::ControlInlet;
};
}

namespace WidgetFactory
{
struct SCORE_ADDON_SPAT_EXPORT SpeakerSetupWidget
{
  static constexpr Process::PortItemLayout layout() noexcept { return {}; }

  static QWidget* make_widget(
      const Gris::SpeakerSetupInlet& inlet, const score::DocumentContext& ctx,
      QWidget* parent, QObject* context);

  static QGraphicsItem* make_item(
      const Gris::SpeakerSetupInlet& slider, const Gris::SpeakerSetupInlet& inlet,
      const score::DocumentContext& ctx, QGraphicsItem* parent, QObject* context);
};
}
