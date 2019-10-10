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
#include <fstream>
#include <string>
#include <thread>
#include <wiringPi.h>
#include <unistd.h>
#include <boost/asio/ip/host_name.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/filereadstream.h"

#include "control_packet.hpp"
#include "comms/mqtt/mqtt_client.hpp"
#include "kkbb01.hpp"
#include "kklp01.hpp"

using namespace std::chrono;

constexpr auto config_cmdarg_short    = "-c";
constexpr auto config_cmdarg_long     = "--config";
constexpr auto conf_debug_key         = "debug_level";
constexpr auto conf_lboard_key        = "light_board";
constexpr auto conf_lboard_spi_key    = "spi_speed";
constexpr auto conf_mqtt_key          = "mqtt";
constexpr auto conf_mqtt_server_key   = "server_address";
constexpr auto conf_mqtt_clientid_key = "client_id";
constexpr auto conf_mqtt_topicurl_key = "topic_url";
constexpr auto conf_mqtt_qos_key      = "QOS";
constexpr auto clientid_replace_key   = "${computer_name}";
constexpr auto topicurl_replace_key   = "${client_id}";
constexpr auto serv_addr_default      = "localhost";

void device_controller(std::future<void> future_obj, Kkbb01 &comm_board, Kklp01 &led_board, const bool &active);
MQTT_Options get_mqtt_options(rapidjson::Document& config_doc);
void set_log_level(rapidjson::Document& config_doc);
Board_Options get_board_options(rapidjson::Document& config_doc);
bool setup(const std::string &config_path, MQTT_Options *comm_opts, Board_Options *brd_options);

int main(int argc, char* argv[])
{
    std::string config_path;
    for (int idx = 1; idx < argc; idx++)
    {
        std::string arg(argv[idx]);
        if (arg.find(config_cmdarg_short) != std::string::npos || arg.find(config_cmdarg_long) != std::string::npos) 
        {
            config_path = std::string(argv[idx + 1]);
            break;
        }
    }

    MQTT_Options comm_opt;
    Board_Options brd_opt;
    bool success = setup(config_path, &comm_opt, &brd_opt);

    if (success)
    {
        std::promise<void> exit_sig;
        std::future<void> future_obj = exit_sig.get_future();

        bool active = false;
        Kklp01 led_board;
        Kkbb01 comm_board(brd_opt);
        std::thread device_t(&device_controller, std::move(future_obj), std::ref(comm_board), std::ref(led_board), active);

        MQTT_Client device_client(comm_opt);
        device_client.connect();

        Control_Packet pack;
        while (true)
        {
            try
            {
                if(device_client.process_message(&pack))
                {
                    switch(pack.data_direction)
                    {
                        case Direction::incoming:
                            BOOST_LOG_TRIVIAL(debug) << "Responding to incoming command message";

                            active = false;
                            
                            led_board.set_effect(pack);

                            active = true;
                        break;

                        case Direction::outgoing:
                            BOOST_LOG_TRIVIAL(debug) << "Sending status update to server.";

                            //TODO: Status sending to connected server upon request.
                        break;

                        default:
                            BOOST_LOG_TRIVIAL(info) << "A control packet was sent that did not contain a value to act upon, skipping.";
                        break;
                    }
                }

                delay(1);
            }
            catch (std::exception &e)
            {
                BOOST_LOG_TRIVIAL(error) << "A problem has occured in the main loop, check logs for additional information." << std::endl
                                         << e.what();
                break;
            }
        }

        device_client.disconnect();
        exit_sig.set_value();

        device_t.join();

        BOOST_LOG_TRIVIAL(info) << "Program is shutting down, turning off the hardware.";
        
        led_board.clear_effect();
        comm_board.write_message(led_board.pack_message());
        comm_board.turn_off();
    }
    else
    {
        BOOST_LOG_TRIVIAL(fatal) << "Setup failed when configuring, check the logs for more information.";
        return 1;
    }

    return 0;
}

bool setup(const std::string &config_path, MQTT_Options *comm_opts, Board_Options *brd_options)
{
    bool result = false;

    if (config_path.length() > 0)
    {
        try
        {
            FILE* fp = fopen(config_path.data(), "r");
            if (fp)
            {
                BOOST_LOG_TRIVIAL(info) << "Config file found, opening '" << config_path << "'.";

                char readBuffer[65536];
                rapidjson::FileReadStream conf_stream(fp, readBuffer, sizeof(readBuffer));
                
                rapidjson::Document config_doc;
                config_doc.ParseStream(conf_stream);
            
                *comm_opts = get_mqtt_options(config_doc);

                *brd_options = get_board_options(config_doc);

                set_log_level(config_doc);

                result = true;
            }
        }
        catch (std::exception &e)
        {
            BOOST_LOG_TRIVIAL(error) << "Setup encountered an error, information is as follows." << std::endl
                                     << e.what(); 
        }
    }
    else
    {
        BOOST_LOG_TRIVIAL(fatal) << "You must specify the location of the config file using -c (--config) <path>.";
    }

    return result;
}

MQTT_Options get_mqtt_options (rapidjson::Document& config_doc)
{
    MQTT_Options result;

    if (config_doc.HasMember(conf_mqtt_key))
    {
        rapidjson::Value& mqtt_val = config_doc[conf_mqtt_key];
        if (mqtt_val.HasMember(conf_mqtt_server_key))
        {
            result.server_address = mqtt_val[conf_mqtt_server_key].GetString();
        }
        else
        {
            result.server_address = serv_addr_default;
        }

        BOOST_LOG_TRIVIAL(info) << "MQTT server address has been set to : " << result.server_address;

        if (mqtt_val.HasMember(conf_mqtt_clientid_key))
        {
            result.client_id = mqtt_val[conf_mqtt_clientid_key].GetString();

            BOOST_LOG_TRIVIAL(debug) << "Client_id has a temp value of :" << result.client_id;

            if (result.client_id.find(clientid_replace_key) != std::string::npos)
            {
                size_t str_idx = result.client_id.find("${");
                size_t stp_idx = result.client_id.find("}", str_idx);

                result.client_id = result.client_id.replace(str_idx, stp_idx - str_idx + 1, boost::asio::ip::host_name());
            }
        }
        else
        {
            result.client_id = boost::asio::ip::host_name();
        }

        BOOST_LOG_TRIVIAL(info) << "MQTT client id has been set to : " << result.client_id;

        if (mqtt_val.HasMember(conf_mqtt_topicurl_key))
        {
            result.topic_url = mqtt_val[conf_mqtt_topicurl_key].GetString();
            if (result.topic_url.find(topicurl_replace_key) != std::string::npos)
            {
                size_t str_idx = result.topic_url.find("${");
                size_t stp_idx = result.topic_url.find("}", str_idx);
                std::string sub_key = result.topic_url.substr(str_idx + 2, (stp_idx - str_idx) - 2);

                result.topic_url = result.topic_url.replace(str_idx, stp_idx - str_idx + 1, result.client_id);
            }
        }

        BOOST_LOG_TRIVIAL(info) << "MQTT topic url has been set to : " << result.topic_url;

        if (mqtt_val.HasMember(conf_mqtt_qos_key))
        {
            result.qos_val = mqtt_val[conf_mqtt_qos_key].GetInt();  
        }

        BOOST_LOG_TRIVIAL(info) << "MQTT quality of service has been set to : " << result.qos_val;
    }

    return result;
}

Board_Options get_board_options(rapidjson::Document& config_doc)
{
    Board_Options opt;
    if (config_doc.HasMember(conf_lboard_key))
    {
        rapidjson::Value& board_val = config_doc[conf_lboard_key];
        if (board_val.HasMember(conf_lboard_spi_key))
        {
            opt.spi_speed = board_val[conf_lboard_spi_key].GetInt();
            BOOST_LOG_TRIVIAL(info) << "Set spi speed to : " << opt.spi_speed; 
        }
        else
        {
            BOOST_LOG_TRIVIAL(info) << "SPI bus speed was not specified, defaulting to 7.8 mhz.";
        }
    }

    return opt;
}

void set_log_level(rapidjson::Document& config_doc)
{
    boost::log::trivial::severity_level level_val = boost::log::trivial::warning;

    if (config_doc.HasMember(conf_debug_key))
    {
        std::map<std::string, boost::log::trivial::severity_level> conv_map = 
        {
            {"trace",   boost::log::trivial::trace},
            {"debug",   boost::log::trivial::debug},
            {"info",    boost::log::trivial::info},
            {"warning", boost::log::trivial::warning},
            {"error",   boost::log::trivial::error},
            {"fatal",   boost::log::trivial::fatal}
        };

        level_val = conv_map[config_doc[conf_debug_key].GetString()];
    }

    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= level_val
    );

    BOOST_LOG_TRIVIAL(info) << "Boost filter has been set to: " << level_val;
}

void device_controller(std::future<void> future_obj, Kkbb01 &comm_board, Kklp01 &led_board, const bool &active)
{
    while (future_obj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
	{
		if (active)
        {
            led_board.incr_frame();
            comm_board.write_message(led_board.pack_message());
        }

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
