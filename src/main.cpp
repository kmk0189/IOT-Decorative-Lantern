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

#include <chrono>
#include <string>
#include <thread>
#include <wiringPi.h>

#include "control_packet.hpp"
#include "iot_client.hpp"
#include "kkbb01.hpp"
#include "kklp01.hpp"

using namespace std::chrono;

constexpr auto SERVER_ADDRESS = "tcp://192.168.12.49:8440";
constexpr auto CLIENTID = "async_consume";
constexpr auto TOPIC = "hello" ;
constexpr int  QOS = 1;
void device_controller(Kkbb01& comm_board, Kklp01& led_board, bool& active);

int main(int argc, char* argv[])
{
    // TODO: use commandline to pass in login information for listener.
    std::vector<char*> arguments(argc);

    for (int idx = 0; idx < argc; idx++)
    {
        arguments[idx] = argv[idx];
    }

    bool active = false;
    Kkbb01 comm_board(7800000);
    Kklp01 led_board;
    IOT_Client device_client;

    std::thread device_t(device_controller, std::ref(comm_board), std::ref(led_board), std::ref(active));

    device_client.start();

    while (true)
    {
        device_client.process_connection();

        if (device_client.has_new_data())
        {
            active = false;

            Control_Packet pack = device_client.get_packet();

            led_board.set_effect(pack.color, pack.group, pack.type);

            active = true;
        }

        delay(1);
    }

    device_t.join();

    return 0;
}

void device_controller(Kkbb01 &comm_board, Kklp01 &led_board, bool &active)
{
    while (true)
    {
        if (active)
        {
            led_board.incr_frame();
            comm_board.write_message(led_board.pack_message());
        }

        delay(1);
    }
}

void delay(unsigned int delaytime)
{
    std::chrono::milliseconds xmillisecs(delaytime);
    std::this_thread::sleep_for(xmillisecs);
}
