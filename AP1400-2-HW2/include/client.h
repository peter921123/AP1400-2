#ifndef CLIENT_H
#define CLIENT_H
#include "crypto.h"
#include <vector>

class Server;

extern std::vector<std::string> pending_trxs;

class Client
{
public:
	Client(std::string id, const Server& server);
	std::string get_id() const;
	std::string get_publickey() const;
	double get_wallet() const;
	std::string sign(std::string txt) const;
	bool transfer_money(std::string receiver, double value);
	size_t generate_nonce() const;
private:
	Server const* const server;
	const std::string id;
	std::string public_key;
	std::string private_key;
};
#endif //CLIENT_H