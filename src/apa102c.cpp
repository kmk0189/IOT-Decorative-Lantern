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

#include "apa102c.hpp"

apa102c::apa102c()
{
    device_color = Color(brightness_mask, 0, 0, 0);
}

apa102c::apa102c(Color color)
{
    device_color = color;
}

uint8_t apa102c::get_blue()
{
    return device_color.get_blue();
}

uint8_t apa102c::get_brightness()
{
    return device_color.get_brightness() | brightness_mask;
}

uint8_t apa102c::get_green()
{
    return device_color.get_green();
}

uint8_t apa102c::get_red()
{
    return device_color.get_red();
}

void apa102c::set_color(Color new_color)
{
    device_color = new_color;
}
