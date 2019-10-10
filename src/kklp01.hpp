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

#include <array>
#include <chrono>
#include <map>
#include <vector>

#include "apa102c.hpp"
#include "effects.hpp"
#include "control_packet.hpp"

using namespace std::chrono;

// the number of led lights on the board.
constexpr auto max_chips = 8;

// The size of a color field (ARGB);
constexpr auto color_size = 4;

// message size for this device 4 byte header + (8 devices * 4 bytes) + 4 byte footer
constexpr auto msg_size_bytes = sizeof(header_val) + (max_chips * color_size) + sizeof(footer_val);

//  This class allows transmission via SPI to attached devices.
class Kklp01
{
public:
    Kklp01();
    ~Kklp01();

    // @returns An array of information on the attached LED chips. 
    std::array<apa102c, max_chips> get_chips();

    //  Advances the effects that are attached to this board.
    void incr_frame();

    //  @returns A vector containing the message for the chips on this board.
    std::vector<uint8_t> pack_message();

    // A
    void set_effect(Control_Packet &pack);
    void set_effect(Color color, Effect_Group group, Effect_Type type);

    void turn_on();
    void turn_off();
    void clear_effect();

private:
    std::array<Effect*, 2> active_effects;
    milliseconds last_frame_time;
    std::array<apa102c, max_chips> led_chips;
    
    Effect* create_effect(Color color, Effect_Group group, Effect_Type type);
    void change_effect(Color color, Effect_Group group, Effect_Type type);
};
