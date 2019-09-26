#include <boost/log/trivial.hpp>

#include "persistence_store.hpp"

Persistence_Store::Persistence_Store() 
	: is_open(false) 
{}

void Persistence_Store::open(const std::string& client_id, const std::string& server_url)
{
	//BOOST_LOG_TRIVIAL(info) << "Opening store for '" << client_id << "' at '" << server_url;

	is_open = true;
}


void Persistence_Store::close() 
{
	//BOOST_LOG_TRIVIAL(info) << "Closing store.";

	is_open = false;
}

void Persistence_Store::clear() 
{
	//BOOST_LOG_TRIVIAL(info) << "Clearing store.";
	
	pointer_map.clear();
}


bool Persistence_Store::contains_key(const std::string &key) 
{
	return pointer_map.find(key) != pointer_map.end();
}


const mqtt::string_collection& Persistence_Store::keys() const 
{
	static mqtt::string_collection ks;

	ks.clear();
	
	for (const auto& k : pointer_map)
	{
		ks.push_back(k.first);
	}

	return ks;
}


void Persistence_Store::put(const std::string& key, const std::vector<mqtt::string_view>& bufs) 
{
	//BOOST_LOG_TRIVIAL(info) << "Persisting data with key '" << key << "'";

	std::string str;
	for (const auto& b : bufs)
	{
		str += b.str();
	}

	pointer_map[key] = std::move(str);
}

mqtt::string_view Persistence_Store::get(const std::string& key) const 
{
	//BOOST_LOG_TRIVIAL(info) << "Searching store for key '" << key << "'";

	auto p = pointer_map.find(key);

	if (p == pointer_map.end())
	{
		throw mqtt::persistence_exception();
	}
	
	//BOOST_LOG_TRIVIAL(info) << "Found store data for key '" << key << "'";

	return mqtt::string_view(p->second);
}

void Persistence_Store::remove(const std::string &key) 
{
	//BOOST_LOG_TRIVIAL(info) << "Store removing key '" << key << "'";

	auto p = pointer_map.find(key);

	if (p == pointer_map.end())
	{
		throw mqtt::persistence_exception();
	}
	
	pointer_map.erase(p);
	
	//BOOST_LOG_TRIVIAL(info) << "Store key '" << key << "' removed.";
}