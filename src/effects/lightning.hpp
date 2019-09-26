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

#include "../color.hpp"
#include "effect.hpp"

class Lightning 
    : public Effect
{
public:
    Lightning(uint16_t number_of_devices, Effect_Group group, Color color);
    Effect_Type get_type() override;
    void set_color(Color new_color) override;
    void incr_frame() override;

private:
    bool bolt_active;
    Color bolt_color;
    unsigned char bolt_brightness;
};