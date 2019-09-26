#pragma once
#include <mqtt/client.h>

class Client_Callback 
    : public virtual mqtt::callback
{
	void connection_lost(const std::string& reason) override;

	void delivery_complete(mqtt::delivery_token_ptr token) override;
};