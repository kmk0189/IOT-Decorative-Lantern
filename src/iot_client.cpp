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

#include "iot_client.hpp"


IOT_Client::IOT_Client()
{
    allow_incoming = false;

}

bool IOT_Client::has_new_data()
{
    return false;
}

Control_Packet IOT_Client::get_packet()
{
    return Control_Packet();
}

void IOT_Client::process_connection()
{
    if (allow_incoming)
    {
       
    }
}

void IOT_Client::start()
{
    allow_incoming = true;
}

void IOT_Client::stop()
{
    allow_incoming = false;
}
