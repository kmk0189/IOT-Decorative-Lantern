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
#define BOOST_TEST_MODULE apa102c
#include <boost/test/unit_test.hpp>

#include "../src/apa102c.hpp"

BOOST_AUTO_TEST_CASE(init_rgb_success)
{
    Color test_color(0x2583A4CE);

    apa102c test_class(test_color);

    BOOST_CHECK_EQUAL((test_color.get_brightness() | brightness_mask), test_class.get_brightness());
    BOOST_CHECK_EQUAL(test_color.get_red(), test_class.get_red());
    BOOST_CHECK_EQUAL(test_color.get_green(), test_class.get_green());
    BOOST_CHECK_EQUAL(test_color.get_blue(), test_class.get_blue());
}