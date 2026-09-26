#pragma once

#include <Process/Process.hpp>

#include <Gris/Algo/SpeakerSetup.hpp>
#include <Gris/Metadata.hpp>
#include <Gris/SpeakerSetupInlet.hpp>

#include <score_addon_spat_export.h>

#include <memory>
#include <string>
#include <verdigris>

namespace Gris
{
struct Layout;
struct LiveSources;

class SCORE_ADDON_SPAT_EXPORT SpatModel final : public Process::ProcessModel
{
  SCORE_SERIALIZE_FRIENDS
  PROCESS_METADATA_IMPL(SpatModel)
  W_OBJECT(SpatModel)

public:
  enum FixedInlet
  {
    SpeakerSetupPort = 0,
    SpeakerList,
    Interpolation,
    AttenuationCutoff,
    AttenuationGain,
    FixedInletCount
  };
  enum SourceInlet
  {
    Audio = 0,
    Position,
    AzimuthSpan,
    ZenithSpan,
    Mode,
    SourceInletCount
  };

  static constexpr int defaultSourceCount = 8;
  static constexpr int maxSourceCount = 128;

  explicit SpatModel(
      const TimeVal& duration, const Id<Process::ProcessModel>& id, QObject* parent);
  ~SpatModel() override;

  template <typename Impl>
  SpatModel(Impl& vis, QObject* parent)
      : Process::ProcessModel{vis, parent}
  {
    vis.writeTo(*this);
    init();
  }

  [[nodiscard]] SpeakerSetupInlet& speakerSetupInlet() const noexcept;
  [[nodiscard]] SpeakerSetup speakerSetup() const noexcept;
  [[nodiscard]] std::string layoutKey() const;
  [[nodiscard]] std::shared_ptr<Layout const> const& layout() const noexcept
  {
    return m_layout;
  }
  [[nodiscard]] std::shared_ptr<LiveSources> const& liveSources() const noexcept
  {
    return m_live;
  }

  [[nodiscard]] int sourceCount() const noexcept { return m_sourceCount; }
  void setSourceCount(int count);
  W_SLOT(setSourceCount);
  void sourceCountChanged(int count) W_SIGNAL(sourceCountChanged, count);

  [[nodiscard]] static int firstInletOf(int source) noexcept
  {
    return FixedInletCount + source * SourceInletCount;
  }

  W_PROPERTY(
      int, sourceCount READ sourceCount WRITE setSourceCount NOTIFY sourceCountChanged)

private:
  void init();
  void prepareLayout();
  void addSourcePorts(int source, int& nextId);

  int m_sourceCount{defaultSourceCount};
  std::shared_ptr<Layout const> m_layout;
  std::shared_ptr<LiveSources> m_live;
};
}
