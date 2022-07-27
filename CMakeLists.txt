cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

if(NOT TARGET score_lib_base)
  include("${SCORE_SOURCE_DIR}/cmake/ScoreExternalAddon.cmake")
endif()

project(score_addon_spat LANGUAGES CXX)

add_library(
  score_addon_spat

  Spat/StereoToMonoModel.hpp
  Spat/StereoToMonoModel.cpp
  Spat/StereoToMonoUi.hpp
  Spat/StereoToMono.hpp

  Spat/StereoPanningModel.hpp
  Spat/StereoPanningModel.cpp
  Spat/StereoPanningUi.hpp
  Spat/StereoPanning.hpp

  Spat/RotatorModel.hpp
  Spat/RotatorModel.cpp
  Spat/RotatorUi.hpp
  Spat/Rotator.hpp

  Spat/AmbiToBinauralModel.hpp
  Spat/AmbiToBinauralModel.cpp
  Spat/AmbiToBinauralUi.hpp
  Spat/AmbiToBinaural.hpp

  Spat/MonoToAmbiModel.hpp
  Spat/MonoToAmbiModel.cpp
  Spat/MonoToAmbiUi.hpp
  Spat/MonoToAmbi.hpp

  Spat/VbapModel.hpp
  Spat/VbapModel.cpp
  Spat/VbapUi.hpp
  Spat/Vbap.hpp

  Spat/_AmbisonicMethods.hpp
  Spat/_AmbisonicMethods.cpp
  Spat/_VbapMethods.hpp
  Spat/_VbapMethods.cpp

  Spat/Spatatouille.hpp
  Spat/SpatatouilleModel.cpp
  Spat/SpatatouilleUi.hpp
  Spat/Spatatouille.hpp

#  Spat/SpatialisationModel.hpp
#  Spat/SpatialisationModel.cpp
#  Spat/SpatialisationUi.hpp
#  Spat/Spatialisation.hpp

  Spat/SpecDisplayModel.hpp
  Spat/SpecDisplayModel.cpp
  Spat/SpecDisplayUi.hpp
  Spat/SpecDisplay.hpp

  Spat/AudioSpatModel.hpp
  Spat/AudioSpatModel.cpp
  Spat/AudioSpatUi.hpp
  Spat/AudioSpat.hpp

  Spat/WidgetSpatatouille.hpp
  Spat/WidgetAudioSpat.hpp
  Spat/WidgetMatrix.hpp
  Spat/WidgetSpectSpat.hpp
  Spat/WidgetDbBar.hpp
  Spat/WidgetDbBarVu.hpp
  Spat/WidgetDomeSpat.hpp
  Spat/WidgetSpecDisplay.hpp

  Spat/WidgetIndex.hpp

  score_addon_spat.hpp
  score_addon_spat.cpp
)

setup_score_plugin(score_addon_spat)

target_link_libraries(score_addon_spat PUBLIC score_plugin_engine score_plugin_avnd)
