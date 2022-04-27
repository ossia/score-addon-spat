#pragma once
#include <Spat/MoscaModel.hpp>
#include <halp/layout.hpp>

namespace Spat
{
struct Mosca::ui
{
  using enum halp::colors;
  using enum halp::layouts;

  halp_meta(name, "Main")
  halp_meta(layout, halp::layouts::grid)
  halp_meta(columns, 3)
  halp_meta(background, "background.svg")
  halp_meta(width, 600)
  halp_meta(height, 300)
  halp_meta(font, "Inconsolata")

  struct {
      halp_meta(name, "Mosca")
      halp_meta(layout, halp::layouts::vbox)
      halp_meta(background, halp::colors::mid)
      halp_meta(width, 300)
      halp_meta(height, 300)

      halp::custom_item<custom_mosca, &ins::level> widget{{.x = 500, .y = 920}};
  } mosca;

  struct {
      halp_meta(name, "Option")
      halp_meta(layout, halp::layouts::vbox)
      halp_meta(width, 300)
      halp_meta(height, 300)

      halp::item<&ins::level> level;
  } option;
};
}
