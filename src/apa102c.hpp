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

#include <stdint.h>

#include "color.hpp"

//  Start header needed to send before the first LED data is sent.
constexpr auto header_val = 0x00000000;

//  end footer that comes after all other data.
constexpr auto footer_val = 0xffffffff;

//  mask used to adjust brightness as the top three bits are fixed for this led device.
constexpr auto brightness_mask = 0xe0;

//  A data class for use with the APA102C (Dotstar) RGB led device.
class apa102c
{
public:
    //  Intitates the class with an off value for the color.
    apa102c();

    //  Initiates the class with a specific color.
    //
    //  @param new_color The color to set on the device.
    apa102c(Color color);

    //  Retreives the device brightness.
    //
    //  @returns The current device brightness or'd with the brightness mask.
    uint8_t get_brightness();

    uint8_t get_red();
    uint8_t get_green();
    uint8_t get_blue();

    //  Sets the color for the device while adjusting brightness to match with
    //  hardware registers.
    //
    //  @param new_color The color to set the device to.
    void set_color(Color new_color);

private:
    Color device_color;
};
