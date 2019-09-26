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

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Kklp01
#include <boost/test/unit_test.hpp>

#include "../src/kklp01.hpp"

BOOST_AUTO_TEST_CASE(set_effect_all_success)
{
    Kklp01 lgt_puck;
    Color test_color(0xFF6C6960);

    lgt_puck.set_effect(test_color, Effect_Group::all, Effect_Type::solid);
    lgt_puck.incr_frame();

    auto lights = lgt_puck.get_chips();

    for (auto &&light : lights)
    {
        BOOST_CHECK_EQUAL(light.get_brightness(), test_color.get_brightness());
        BOOST_CHECK_EQUAL(light.get_red(), test_color.get_red());
        BOOST_CHECK_EQUAL(light.get_green(), test_color.get_green());
        BOOST_CHECK_EQUAL(light.get_blue(), test_color.get_blue());
    }
}

BOOST_AUTO_TEST_CASE(set_effect_odd_success)
{
    Kklp01 lgt_puck;
    Color test_color(0xFFF5D033);

    lgt_puck.set_effect(test_color, Effect_Group::odd, Effect_Type::solid);
    lgt_puck.incr_frame();

    auto lights = lgt_puck.get_chips();

    for (size_t idx = 0; idx < lights.size(); idx++)
    {
        if(idx % 2 == 0)
        {
            BOOST_CHECK_NE(lights[idx].get_brightness(), test_color.get_brightness());
            BOOST_CHECK_NE(lights[idx].get_red(), test_color.get_red());
            BOOST_CHECK_NE(lights[idx].get_green(), test_color.get_green());
            BOOST_CHECK_NE(lights[idx].get_blue(), test_color.get_blue());
        }
        else
        {    
            BOOST_CHECK_EQUAL(lights[idx].get_brightness(), test_color.get_brightness() | brightness_mask);
            BOOST_CHECK_EQUAL(lights[idx].get_red(), test_color.get_red());
            BOOST_CHECK_EQUAL(lights[idx].get_green(), test_color.get_green());
            BOOST_CHECK_EQUAL(lights[idx].get_blue(), test_color.get_blue());
        }
    }
}

BOOST_AUTO_TEST_CASE(set_effect_even_success)
{
    Kklp01 lgt_puck;
    Color test_color(0xFF2F353B);

    lgt_puck.set_effect(test_color, Effect_Group::all, Effect_Type::solid);
    lgt_puck.incr_frame();

    auto lights = lgt_puck.get_chips();

    for (size_t idx = 0; idx < lights.size(); idx++)
    {
        if(idx % 2 == 0)
        {
            BOOST_CHECK_EQUAL(lights[idx].get_brightness(), test_color.get_brightness() | brightness_mask);
            BOOST_CHECK_EQUAL(lights[idx].get_red(), test_color.get_red());
            BOOST_CHECK_EQUAL(lights[idx].get_green(), test_color.get_green());
            BOOST_CHECK_EQUAL(lights[idx].get_blue(), test_color.get_blue());
        }
        else
        {    
            BOOST_CHECK_NE(lights[idx].get_brightness(), test_color.get_brightness() | brightness_mask);
            BOOST_CHECK_NE(lights[idx].get_red(), test_color.get_red());
            BOOST_CHECK_NE(lights[idx].get_green(), test_color.get_green());
            BOOST_CHECK_NE(lights[idx].get_blue(), test_color.get_blue());
        }
    }
}

BOOST_AUTO_TEST_CASE(test_message_generation)
{
    Kklp01 lgt_puck;
    Color test_color(0xFF8F8B66);

    lgt_puck.set_effect(test_color, Effect_Group::all, Effect_Type::solid);
    lgt_puck.incr_frame();

    std::vector<unsigned char> msg_generated = lgt_puck.pack_message();

    unsigned long header = 0;
    header |= msg_generated[0] << 24;
    header |= msg_generated[1] << 16;
    header |= msg_generated[2] << 8;
    header |= msg_generated[3];
    BOOST_CHECK_EQUAL(header, header_val);

    for (int dev_idx = max_chips - 1; dev_idx >= 0; dev_idx--)
    {
        BOOST_CHECK_EQUAL(msg_generated[dev_idx], test_color.get_brightness() | brightness_mask);
        BOOST_CHECK_EQUAL(msg_generated[dev_idx], test_color.get_red());
        BOOST_CHECK_EQUAL(msg_generated[dev_idx], test_color.get_blue());
        BOOST_CHECK_EQUAL(msg_generated[dev_idx], test_color.get_green());
    }

    unsigned long footer = 0;
    int offset = sizeof(header_val) + (max_chips * color_size);
    footer |= msg_generated[offset] << 24;
    footer |= msg_generated[offset + 1] << 16;
    footer |= msg_generated[offset + 2] << 8;
    footer |= msg_generated[offset + 3];
    BOOST_CHECK_EQUAL(footer, header_val);
}