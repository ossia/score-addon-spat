cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

if(NOT TARGET score_lib_base)
  include(ScoreExternalAddon)
endif()

project(score_addon_spat LANGUAGES CXX)

score_common_setup()

find_package(${QT_VERSION} REQUIRED COMPONENTS Core Widgets)

set(HDRS
  Gris/Algo/GainMatrix.hpp
  Gris/Algo/Mbap.hpp
  Gris/Algo/Quaternion.hpp
  Gris/Algo/SpeakerSetup.hpp
  Gris/Algo/SpeakerSetupIO.hpp
  Gris/Algo/Spatializer.hpp
  Gris/Algo/Types.hpp
  Gris/Algo/Vbap.hpp
  Gris/Commands.hpp
  Gris/Executor.hpp
  Gris/Inspector.hpp
  Gris/Layer.hpp
  Gris/LiveSources.hpp
  Gris/LayoutCache.hpp
  Gris/Metadata.hpp
  Gris/Model.hpp
  Gris/SpeakerList.hpp
  Gris/SpatView.hpp
  Gris/SpeakerSetupInlet.hpp

  Spat/StereoToMonoModel.hpp
  Spat/StereoToMonoUi.hpp
  Spat/StereoToMono.hpp
  Spat/StereoPanningModel.hpp
  Spat/StereoPanningUi.hpp
  Spat/StereoPanning.hpp
  Spat/RotatorModel.hpp
  Spat/RotatorUi.hpp
  Spat/Rotator.hpp
  Spat/AmbiToBinauralModel.hpp
  Spat/AmbiToBinauralUi.hpp
  Spat/AmbiToBinaural.hpp
  Spat/MonoToAmbiModel.hpp
  Spat/MonoToAmbiUi.hpp
  Spat/MonoToAmbi.hpp
  Spat/_AmbisonicMethods.hpp
  Spat/SpatatouilleModel.hpp
  Spat/SpatatouilleUi.hpp
  Spat/Spatatouille.hpp
  Spat/WidgetSpatatouille.hpp
  Spat/WidgetAudioSpat.hpp
  Spat/WidgetMatrix.hpp
  Spat/WidgetSpectSpat.hpp
  Spat/WidgetDbBar.hpp
  Spat/WidgetDbBarVu.hpp
  Spat/WidgetDomeSpat.hpp
  Spat/WidgetIndex.hpp

  score_addon_spat.hpp
)

set(SRCS
  Gris/Algo/Mbap.cpp
  Gris/Algo/Quaternion.cpp
  Gris/Algo/SpeakerSetup.cpp
  Gris/Algo/SpeakerSetupIO.cpp
  Gris/Algo/Spatializer.cpp
  Gris/Algo/Vbap.cpp
  Gris/Commands.cpp
  Gris/Executor.cpp
  Gris/Layer.cpp
  Gris/LayoutCache.cpp
  Gris/Model.cpp
  Gris/Serialization.cpp
  Gris/SpeakerList.cpp
  Gris/SpatView.cpp
  Gris/SpeakerSetupInlet.cpp

  Spat/StereoToMonoModel.cpp
  Spat/StereoPanningModel.cpp
  Spat/RotatorModel.cpp
  Spat/AmbiToBinauralModel.cpp
  Spat/MonoToAmbiModel.cpp
  Spat/_AmbisonicMethods.cpp
  Spat/SpatatouilleModel.cpp

  score_addon_spat.cpp
)

add_library(score_addon_spat ${SRCS} ${HDRS})

score_generate_command_list_file(score_addon_spat "${HDRS}")

target_include_directories(score_addon_spat PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(score_addon_spat
  PUBLIC ${QT_PREFIX}::Core ${QT_PREFIX}::Widgets
         score_lib_base score_lib_process score_lib_state score_lib_inspector
         score_plugin_engine score_plugin_dataflow score_plugin_avnd
)

setup_score_plugin(score_addon_spat)
