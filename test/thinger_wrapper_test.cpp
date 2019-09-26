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
#define BOOST_TEST_MODULE thinger_wrapper
#include <boost/test/unit_test.hpp>

#include "../src/thinger_wrapper.cpp"

BOOST_AUTO_TEST_CASE(test_handle_incoming)
{
    Color test_color(0xFFC72247);

    handle_incoming(test_color.get_brightness(), test_color.get_red(), test_color.get_green(), test_color.get_blue(), (uint8_t)Effect_Type::solid, (uint8_t)Effect_Group::all);

    Control_Packet pckt = get_thinger_packet();

    BOOST_CHECK_EQUAL(pckt.color, test_color);
    BOOST_CHECK_EQUAL(pckt.type, Effect_Type::solid);
    BOOST_CHECK_EQUAL(pckt.group, Effect_Group::all);
}