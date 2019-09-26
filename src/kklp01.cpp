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

#include "kklp01.hpp"

Kklp01::Kklp01() 
{
    std::fill(active_effects.begin(), active_effects.end(), nullptr);
}

Kklp01::~Kklp01()
{
    for (auto& active_effect : active_effects)
    {
        delete active_effect;
        active_effect = nullptr;
    }
}

void Kklp01::set_effect(Color color, Effect_Group group /*= Effect_Group::all*/, Effect_Type type /*= Effect_Type::single*/)
{
    if (group == Effect_Group::all)
    {
        set_effect(color, Effect_Group::even, type);
        set_effect(color, Effect_Group::odd, type);
    }
    else
    {
        set_effect(color, group, type);
    }
}

//  Changes the effect or creates a new one on the specified group.
void Kklp01::change_effect(Color color, Effect_Group group, Effect_Type type)
{
    if (active_effects[(uint8_t)group] != nullptr && active_effects[(uint8_t)group]->get_type() == type)
    {
        active_effects[(uint8_t)group]->set_color(color);
    }
    else if (active_effects[(uint8_t)group] != nullptr && active_effects[(uint8_t)group]->get_type() != type)
    {
        delete active_effects[(uint8_t)group];

        active_effects[(uint8_t)group] = create_effect(color, group, type);
    }
    else
    {
        active_effects[(uint8_t)group] = create_effect(color, group, type);
    }
}

Effect* Kklp01::create_effect(Color color, Effect_Group group, Effect_Type type)
{
    Effect* new_effect = nullptr;
    
    switch (type)
    {
    case Effect_Type::blink:
        break;
    case Effect_Type::bounce:
        break;
    case Effect_Type::lightning:
        break;
    case Effect_Type::raceway:
        break;
    case Effect_Type::rainbow:
        break;
    case Effect_Type::solid:
        new_effect = new Solid(max_chips, group, color);
        break;
    case Effect_Type::stars:
        break;
    case Effect_Type::none:
        break;
    }

    return new_effect;
}

void Kklp01::incr_frame()
{
    milliseconds delta = duration_cast<milliseconds>(system_clock::now().time_since_epoch() - last_frame_time);

    // Loop through current active effects. Each of these loops is one 'Frame' and
    // will run continously.
    // Each effect can have a delay before the next frame is active so we check for 
    // it and only act when it has expired. Note: This is not exact timing due to the
    // nature of this method but should be close enough.
    for (auto& active_effect : active_effects)
    {
        if (active_effect != nullptr && active_effect->get_delay() <= delta.count())
        {
            for (auto& [dev_idx, effect_color] : active_effect->get_device_map())
            {
                led_chips[dev_idx].set_color(effect_color);
            }

            // Increment the 'Frame' within the effect.
            active_effect->incr_frame();
        }
    }
        
    last_frame_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
}

std::array<apa102c, max_chips> Kklp01::get_chips()
{
    return led_chips;
}

std::vector<uint8_t> Kklp01::pack_message()
{
    std::vector<unsigned char> message(msg_size_bytes) ;
    unsigned char idx = 0;

    message[idx++] = (header_val >> 24) & 255;
    message[idx++] = (header_val >> 16) & 255;
    message[idx++] = (header_val >> 8) & 255;
    message[idx++] = header_val & 255;

    for (int dev_idx = max_chips - 1; dev_idx >= 0; dev_idx--)
    {
        // This particular puck uses DotStar LEDs which have a RBG schema.
        message[idx++] = led_chips[dev_idx].get_brightness();
        message[idx++] = led_chips[dev_idx].get_red();
        message[idx++] = led_chips[dev_idx].get_blue();
        message[idx++] = led_chips[dev_idx].get_green();
    }

    message[idx++] = (footer_val >> 24) & 255;
    message[idx++] = (footer_val >> 16) & 255;
    message[idx++] = (footer_val >> 8) & 255;
    message[idx] = footer_val & 255;

    return message;
}
