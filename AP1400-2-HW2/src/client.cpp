#include "client.h"
#include "server.h"
#include <random>
#include <sstream>
#include <iomanip>

Client::Client(std::string id, const Server& server)
    : id(id), server(&server) {

    crypto::generate_key(public_key, private_key);
}

std::string Client::get_id() const {
    return id;
}

std::string Client::get_publickey() const {
    return public_key;
}

double Client::get_wallet() const {
    return server->get_wallet(id);
}

std::string Client::sign(std::string txt) const {
    std::string signature = crypto::signMessage(private_key, txt);
    return signature;
}

bool Client::transfer_money(std::string receiver, double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    std::string trx = id + "-" + receiver + "-" + oss.str();
    std::string signature = sign(trx);
    return server->add_pending_trx(trx, signature);
}

size_t Client::generate_nonce() const {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<size_t> dis;
    return dis(gen);
}