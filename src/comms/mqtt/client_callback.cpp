#include <boost/log/trivial.hpp>
#include "client_callback.hpp"

void Client_Callback::connection_lost(const std::string& reason) 
{
    //BOOST_LOG_TRIVIAL(warning) << "Connection lost";
    
    if (!reason.empty())
    {
        //BOOST_LOG_TRIVIAL(warning) << "Reason: " << reason;
    }
}

void Client_Callback::delivery_complete(mqtt::delivery_token_ptr token) 
{
    //BOOST_LOG_TRIVIAL(info) << "Delivery complete for token: " << (token ? token -> get_message_id() : -1);
}