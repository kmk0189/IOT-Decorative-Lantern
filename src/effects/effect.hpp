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

#include <map>
#include <vector>

#include "../color.hpp"

//  Describes which set of the total light devices are used.
enum class Effect_Group
: uint8_t
{
    all,
    even,
    odd
};

//  Indicates which effect to use on the light devices, it is paired with LED_Group.
enum class Effect_Type
: uint8_t
{
    none,
    blink,
    bounce,
    lightning,
    rainbow,
    raceway,
    solid,
    stars
};

//  Base class for any derived effect the device uses.
class Effect
{
public:
    //  Makes sure that this and all derived classes remove any resources.
    virtual ~Effect() noexcept;

    //  Retreives a list of device colors from the effect, this allows all devices to be written at one time.
    //
    //  @return A standard map of device colors indexed by the device id.
    std::map<uint16_t, Color> get_device_map();

    //  The amount of time that needs to pass before the next step of the effect should happen.
    //
    //  @return An integer representing the amount of time to wait in milliseconds (ms).
    unsigned short get_delay();

    //  Retreives the current effect mode, this is overriden in derived classes.
    //
    //  @return The current mode for the effect.
    virtual Effect_Type get_type();

    //  Increments the effect one frame.
    virtual void incr_frame();

    //  Set the color for the effect.
    virtual void set_color(Color color);

protected:
    //  Maps the device to a color which is used in the next frame.
    std::map<uint16_t, Color> device_map;

    uint16_t frame_delay;
    uint16_t last_device;
    uint16_t max_devices;

    //  Sets up base class when called from a derived object.
    //
    //  @param max_devices The upper limit of devices the delay should consider when constructing a frame.
    //  @param delay The amount of time between each frame in milliseconds.
    //  @param led_group Which devices the effect should operate on.
    Effect(uint16_t max_devices, uint16_t delay, Effect_Group group = Effect_Group::all, Color color = Color());
};
