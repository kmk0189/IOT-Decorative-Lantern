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
#define BOOST_TEST_MODULE Color
#include <boost/test/unit_test.hpp>

#include "../src/color.hpp"

BOOST_AUTO_TEST_CASE(init_hex_success)
{
    const unsigned long test_color = 0x02FFEEDD;

    Color test_class(test_color);

    BOOST_CHECK_EQUAL(test_color, test_class.get_hex());
}

BOOST_AUTO_TEST_CASE(init_rgb_success)
{
    const unsigned char brightness = 0x05;
    const unsigned char red = 0xDD;
    const unsigned char green = 0xA9;
    const unsigned char blue = 0xB7;

    Color test_class(brightness, red, green, blue);

    BOOST_CHECK_EQUAL(brightness, test_class.get_brightness());
    BOOST_CHECK_EQUAL(red, test_class.get_red());
    BOOST_CHECK_EQUAL(green, test_class.get_green());
    BOOST_CHECK_EQUAL(blue, test_class.get_blue());
}
