#pragma once
#include <memory>
#include <mqtt/client.h>
#include "persistence_store.hpp"

class MQTT_Client
{
public:
    MQTT_Client(std::string server_address, std::string client_id, std::string topic_url);

    bool get_connected();

    bool connect();
    bool reconnect();
    bool disconnect();

    void send_message(std::string payload);
    std::string process_message();
    

private:
    std::string base_url;
    Persistence_Store data_store;
    std::unique_ptr<mqtt::client> data_client;
};