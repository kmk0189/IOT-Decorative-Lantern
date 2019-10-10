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

#include <boost/log/trivial.hpp>
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "mqtt_client.hpp"

constexpr auto command_channel      = "/command";
constexpr auto status_channel       = "/status";
constexpr auto effect_group_key     = "effect_group";
constexpr auto effect_type_key      = "effect_type";
constexpr auto color_node_key       = "color";
constexpr auto color_node_brightness_key = "brightness";
constexpr auto color_node_red_key   = "red";
constexpr auto color_node_green_key = "green";
constexpr auto color_node_blue_key  = "blue";
constexpr auto data_direction_key   = "data_direction";

MQTT_Client::MQTT_Client(const MQTT_Options& opt)
{
	base_url = opt.topic_url;

	qos_val = opt.qos_val;
	
	data_client = std::make_unique<mqtt::client>(opt.server_address, opt.client_id);
}

bool MQTT_Client::connect()
{
	if (!get_connected())
	{
		BOOST_LOG_TRIVIAL(info) << "Connecting...";

		mqtt::connect_options connOpts;
		connOpts.set_keep_alive_interval(20);
		connOpts.set_clean_session(true);

		try
		{
			data_client->connect(connOpts);

			BOOST_LOG_TRIVIAL(info) << "Ok!";

			BOOST_LOG_TRIVIAL(info) << "Subscribing to command topic '" << base_url + command_channel << "'.";
			data_client->subscribe(base_url + command_channel, qos_val);
			BOOST_LOG_TRIVIAL(info) << "Ok!";
		}
		catch (const mqtt::exception& exc) 
		{
			BOOST_LOG_TRIVIAL(error) << exc.what();
		}
	}

	return get_connected();
}

bool MQTT_Client::disconnect()
{
	if (get_connected())
	{
		BOOST_LOG_TRIVIAL(info) << "Disconnecting from server...";

		try
		{
			data_client->disconnect();

			BOOST_LOG_TRIVIAL(info) << "Disconnected.";
		}
		catch (const mqtt::exception& exc) 
		{
			BOOST_LOG_TRIVIAL(error) << exc.what();
		}
	}

	return !get_connected();
}

bool MQTT_Client::reconnect()
{
	bool result = false;

	constexpr int max_attempt = 15;
	for (int idx = 0; idx < max_attempt && !get_connected(); idx++) 
	{
		try 
		{
			data_client->reconnect();
			
			result = true;
			break;
		}
		catch (const mqtt::exception& exc) 
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));

			BOOST_LOG_TRIVIAL(warning) << "There was a problem reconnecting to the server." << std::endl << exc.what();
		}
	}

	return result;
}

void MQTT_Client::send_message(const Control_Packet &pack)
{
	rapidjson::Document status_doc;

	status_doc[effect_group_key].SetInt((int)pack.group);
	status_doc[effect_type_key].SetInt((int)pack.type);

	rapidjson::Value& c_node = status_doc[color_node_key];
	c_node[color_node_brightness_key].SetInt(pack.color.get_brightness());
	c_node[color_node_red_key].SetInt(pack.color.get_red());
	c_node[color_node_green_key].SetInt(pack.color.get_green());
	c_node[color_node_blue_key].SetInt(pack.color.get_blue());

	rapidjson::StringBuffer payload;
    rapidjson::Writer<rapidjson::StringBuffer> writer(payload);
    status_doc.Accept(writer);

	BOOST_LOG_TRIVIAL(trace) << "Sending message to: " << base_url + status_channel;

	auto pubmsg = mqtt::make_message(base_url + status_channel, payload.GetString());
	pubmsg->set_qos(qos_val);
	data_client->publish(pubmsg);
}

bool MQTT_Client::get_connected()
{
	return data_client->is_connected();
}

bool MQTT_Client::process_message(Control_Packet* data)
{
	bool result = false;
	auto rx_message = data_client->consume_message();

	if (rx_message)
	{
		std::string msg(rx_message->to_string());
		BOOST_LOG_TRIVIAL(debug) << "Message received, unpacking received json string [" << msg << "].";

		rapidjson::Document command_doc;

    	command_doc.Parse(msg.data());

		if (command_doc.HasMember(color_node_key))
		{
			uint8_t brightness, red, green, blue;
			rapidjson::Value& node = command_doc[color_node_key];

			if (node.HasMember(color_node_brightness_key))
			{
				brightness = node[color_node_brightness_key].GetInt();
			}
			else
			{
				brightness = 0;
			}

			if (node.HasMember(color_node_red_key))
			{
				red = node[color_node_red_key].GetInt();
			}
			else
			{
				red = 0;
			}

			if (node.HasMember(color_node_green_key))
			{
				green = node[color_node_green_key].GetInt();	
			}
			else
			{
				green = 0;
			}

			if (node.HasMember(color_node_blue_key))
			{
				blue = node[color_node_blue_key].GetInt();
			}
			else
			{
				blue = 0;
			}

			data->color = Color(red, green, blue, brightness);

			BOOST_LOG_TRIVIAL(debug) << "Brightness value set to: " << (int)data->color.get_brightness();
			BOOST_LOG_TRIVIAL(debug) << "Red value set to: " << (int)data->color.get_red();
			BOOST_LOG_TRIVIAL(debug) << "Green value set to: " << (int)data->color.get_green();
			BOOST_LOG_TRIVIAL(debug) << "Blue value set to: " << (int)data->color.get_blue();
		}

		if (command_doc.HasMember(effect_group_key))
		{
			data->group = (Effect_Group)command_doc[effect_group_key].GetInt();
		}
		else
		{
			data->group = Effect_Group::all;
		}
		
		BOOST_LOG_TRIVIAL(debug) << "Effect group has been set to: " << (int)data->group; 

		if (command_doc.HasMember(effect_type_key))
		{
			data->type = (Effect_Type)command_doc[effect_type_key].GetInt();
		}
		else
		{
			data->type = Effect_Type::solid;
		}

		BOOST_LOG_TRIVIAL(debug) << "Effect type has been set to: " << (int)data->type;

		if (command_doc.HasMember(data_direction_key))
		{
			data->data_direction = (Direction)command_doc[data_direction_key].GetInt();
		}
		else
		{
			data->data_direction = Direction::none;
		}

		BOOST_LOG_TRIVIAL(debug) << "Data direction has been set to: " << (int)data->data_direction;

		result = true;
	}
	else
	{
		if (!get_connected()) 
		{
			BOOST_LOG_TRIVIAL(warning) << "Lost connection. Attempting reconnect";

			if (reconnect()) 
			{
				data_client->subscribe(base_url + command_channel, qos_val);
			}
		}
		else 
		{
			BOOST_LOG_TRIVIAL(warning) << "An error occurred retrieving messages.";
		}
	}

	return result;
}