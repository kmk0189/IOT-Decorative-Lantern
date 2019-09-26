/*
**  Copyright (c) 2019, Kasey Koscianski.
*****************************************************************************
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU Affero General Public License as published
**  by the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Affero General Public License for more details.
**
**  You should have received a copy of the GNU Affero General Public License
**  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*****************************************************************************
*/

#pragma once
#include "effect.hpp"

//  An effect where all light devices stay lit to the set color and only change when
//  a new color has been set.
class Solid :
    public Effect
{
public:
    //  Creates the effect to create a solid color display.
    //
    //  @param number_of_devices The number of devices to use when setting up the effect.
    //  @param group The light group to create the effect for.
    //  @param color The color to show when the effect is created.
    Solid(uint16_t number_of_devices, Effect_Group group, Color color);

    //  Overriden from the base class to solid for use when adding the effect.
    //
    //  @return The solid effect mode.
    Effect_Type get_type() override;

    //  Changes the effect's color for the next frame.
    //
    //  @param new_color The new color value to use.
    void set_color(Color new_color) override;
};
