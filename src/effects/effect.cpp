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

#include "effect.hpp"

Effect::Effect(uint16_t num_of_devices, uint16_t delay, Effect_Group group, Color color)
{
    max_devices = num_of_devices;
    frame_delay = delay;
    last_device = 0;

    uint16_t device_idx = 0;
    uint8_t incr_amount = 1;

    if (group == Effect_Group::even)
    {
        incr_amount = 2;
    }
    else if (group == Effect_Group::odd)
    {
        device_idx = 1;
        incr_amount = 2;
    }

    while (device_idx < max_devices)
    {
        device_map[device_idx] = color;

        device_idx += incr_amount;
    }
}

Effect::~Effect() {}

void Effect::set_color(Color new_color) {}

void Effect::incr_frame() {}

Effect_Type Effect::get_type()
{
    return Effect_Type::none;
}

std::map<uint16_t, Color> Effect::get_device_map()
{
    return device_map;
}

uint16_t Effect::get_delay()
{
    return frame_delay;
}