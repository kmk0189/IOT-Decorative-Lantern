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

#include "solid.hpp"

constexpr uint16_t frame_delay = 0;

Solid::Solid(uint16_t number_of_devices, Effect_Group group, Color color)
    : Effect(number_of_devices, 0, group, color)
{}

Effect_Type Solid::get_type()
{
    return Effect_Type::solid;
}

void Solid::set_color(Color new_color)
{
    for (auto& [dev_idx, effect_color] : device_map)
    {
        effect_color = new_color;
    }
}