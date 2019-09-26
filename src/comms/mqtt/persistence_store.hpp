#include <mqtt/client.h>

class Persistence_Store 
    : virtual public mqtt::iclient_persistence
{
public:
	Persistence_Store();

	// "Open" the store
	void open(const std::string& client_id, const std::string& server_url) override;

	// Close the persistent store that was previously opened.
	void close() override;

	// Clears persistence, so that it no longer contains any persisted data.
	void clear() override;

	// Returns whether or not data is persisted using the specified key.
	bool contains_key(const std::string &key) override;

	// Returns the keys in this persistent data store.
	const mqtt::string_collection& keys() const override;

	// Puts the specified data into the persistent store.
	void put(const std::string& key, const std::vector<mqtt::string_view>& data_buf) override;

	// Gets the specified data out of the persistent store.
	mqtt::string_view get(const std::string& key) const override;

	// Remove the data for the specified key.
	void remove(const std::string &key) override;

private:
	bool is_open;

	std::map<std::string, std::string> pointer_map;
};