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

#include "lightning.hpp"

constexpr uint16_t frame_delay = 0;

Lightning::Lightning(uint16_t number_of_devices, Effect_Group group, Color color)
    : Effect(number_of_devices, 0, group), bolt_color(color)
{
    bolt_active = false;
    bolt_brightness = color.get_brightness();
}

void Lightning::incr_frame()
{
    if (bolt_active != true)
    {
        bolt_active = true;



        frame_delay = 1;
    }
}

Effect_Type Lightning::get_type()
{
    return Effect_Type::lightning;
}

void Lightning::set_color(Color new_color)
{
    for (auto& [dev_idx, effect_color] : device_map)
    {
        effect_color = new_color;
    }
}
