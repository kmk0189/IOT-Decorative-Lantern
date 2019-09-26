#include <iostream>
#include <chrono>
#include <cstdlib>
#include <string>
#include <thread>
#include <map>
#include <vector>
#include <cstring>
#include <boost/log/trivial.hpp>

#include "mqtt_client.hpp"
#include "client_callback.hpp"

constexpr int QOS = 1;

MQTT_Client::MQTT_Client(std::string server_address, std::string client_id, std::string topic_url)
{
	base_url = topic_url;
	
	data_client = std::make_unique<mqtt::client>(server_address, client_id, &data_store);
}

bool MQTT_Client::connect()
{
	if (!get_connected())
	{
		////BOOST_LOG_TRIVIAL(info) << "Connecting...";

		Client_Callback callback;
		data_client->set_callback(callback);

		mqtt::connect_options connOpts;
		connOpts.set_keep_alive_interval(20);
		connOpts.set_clean_session(true);

		try
		{
			data_client->connect(connOpts);

			//BOOST_LOG_TRIVIAL(info) << "Ok!";
		}
		catch (const mqtt::exception& exc) 
		{
			//BOOST_LOG_TRIVIAL(error) << exc.what();
		}
	}

	return get_connected();
}

bool MQTT_Client::disconnect()
{
	if (get_connected())
	{
		//BOOST_LOG_TRIVIAL(info) << "Disconnecting from server...";

		try
		{
			data_client->disconnect();

			//BOOST_LOG_TRIVIAL(info) << "Disconnected.";
		}
		catch (const mqtt::exception& exc) 
		{
			//BOOST_LOG_TRIVIAL(error) << exc.what();
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
		catch (const mqtt::exception&) 
		{
			// TODO: log about reconnection.
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	return result;
}

void MQTT_Client::send_message(std::string payload)
{
	//BOOST_LOG_TRIVIAL(trace) << "Sending message to: " << topic;

	data_client->publish(mqtt::message(base_url + "/status", payload, QOS, false));
}

bool MQTT_Client::get_connected()
{
	return data_client->is_connected();
}

std::string MQTT_Client::process_message()
{
	auto rx_message = data_client->consume_message();

	if (!rx_message) 
	{
		if (!get_connected()) 
		{
			//cout << "Lost connection. Attempting reconnect" << endl;

			if (reconnect()) 
			{
				data_client->subscribe(base_url + "/command", QOS);

				//cout << "Reconnected" << endl;
			}
			else 
			{
				//cout << "Reconnect failed." << endl;
				//TODO: Periodic reconnect try
			}
		}
		else 
		{
			//cout << "An error occurred retrieving messages." << endl;
		}
	}

	return rx_message->to_string();
}

// TODO: add consuming code.