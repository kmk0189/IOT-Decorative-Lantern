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
#include <memory>
#include <mqtt/client.h>
#include "../../control_packet.hpp"

struct MQTT_Options
{
    std::string server_address;
    std::string client_id;
    std::string topic_url;
    uint8_t qos_val;
};

class MQTT_Client
{
public:
    MQTT_Client(const MQTT_Options& opt);

    bool get_connected();

    bool connect();
    bool reconnect();
    bool disconnect();

    void send_message(const Control_Packet &pack);
    bool process_message(Control_Packet* data);

private:
    std::string base_url;
    uint8_t qos_val;
    std::unique_ptr<mqtt::client> data_client;
};