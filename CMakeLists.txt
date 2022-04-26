cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

if(NOT TARGET score_lib_base)
  include("${SCORE_SOURCE_DIR}/cmake/ScoreExternalAddon.cmake")
endif()

project(score_addon_spat LANGUAGES CXX)

add_library(
  score_addon_spat
  Spat/AnEffectModel.hpp
  Spat/AnEffectModel.cpp
  Spat/AnEffectUi.hpp
  Spat/AnEffect.hpp

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


  score_addon_spat.hpp
  score_addon_spat.cpp
)

setup_score_plugin(score_addon_spat)

target_include_directories(score_plugin_avnd
    PUBLIC
    "${3RDPARTY_FOLDER}/avendish/include"
    /home/scrime/Documents/SAF/Spatial_Audio_Framework/framework/include
    /home/scrime/Documents/SAF/Spatial_Audio_Framework/examples/include
)

target_link_libraries(
    score_addon_spat
    PUBLIC
    score_plugin_engine
    score_plugin_avnd
    PRIVATE
    /home/scrime/Documents/SAF/Spatial_Audio_Framework/build/framework/libsaf.a
    /home/scrime/Documents/SAF/Spatial_Audio_Framework/build/examples/libsaf_example_rotator.a
    -lopenblas
    -llapack
    -llapacke
)
