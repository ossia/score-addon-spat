#pragma once

#include <Spat/WidgetIndex.hpp>

namespace Spat
{

struct custom_db_bar
{
    static constexpr double width() { return 300.; } // Axe X
    static constexpr double height() { return 300.; } // Axe Y

    void paint(avnd::painter auto ctx)
    {

    }

    bool mouse_press(double x, double y, auto button)
    {
        return true;
    }

    void mouse_move(double x, double y, auto button)
    {
    }

    void mouse_release(double x, double y, auto button)
    {
    }
};
}
