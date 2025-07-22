#include "server.h"
#include "client.h"
#include <random>
#include <string>
Server::Server() {;}

std::vector<std::string> pending_trxs;

std::shared_ptr<Client> Server::add_client(std::string id) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);
    
    std::string tempId = id;
    auto isIdEqual = [&id](const auto& clientPair){ return (clientPair.first)->get_id() == id; };
    auto iter = std::find_if(clients.begin(), clients.end(), isIdEqual);
    while (iter != clients.end()) {
        std::string randomFourdigit = std::to_string(dis(gen));
        id = tempId + randomFourdigit;
        iter = std::find_if(clients.begin(), clients.end(), isIdEqual);
    }
    
    std::shared_ptr<Client> ptrClient = std::make_shared<Client>(id, *this);
    clients.insert(std::make_pair(ptrClient, 5.0));
    return ptrClient;
}

std::shared_ptr<Client> Server::get_client(std::string id) const {
    auto isIdEqual = [id](const auto& clientPair){ return (clientPair.first)->get_id() == id; };
    auto iter = std::find_if(clients.begin(), clients.end(), isIdEqual);
    return (iter == clients.end()) ? nullptr : iter->first;
}

double Server::get_wallet(std::string id) const {
    auto isIdEqual = [id](const auto& clientPair){ return (clientPair.first)->get_id() == id; };
    auto iter = std::find_if(clients.begin(), clients.end(), isIdEqual);
    return iter->second;
}

bool Server::parse_trx(std::string trx, std::string& sender, std::string& receiver, double& value) {
    auto iter1 = std::find(trx.begin(), trx.end(), '-');
    auto iter2 = std::find(iter1 + 1, trx.end(), '-');

    if (iter1 == trx.end() || iter2 == trx.end()) throw std::runtime_error("wrong form");

    /*
    std::copy(trx.begin(), iter1, std::back_inserter(sender));
    std::copy(iter1 + 1, iter2, std::back_inserter(receiver));
    std::string stringValue;
    std::copy(iter2 + 1, trx.end(), std::back_inserter(stringValue));
    */
    
    sender.assign(trx.begin(), iter1);
    receiver.assign(iter1 + 1, iter2);
    std::string stringValue;
    stringValue.assign(iter2 + 1, trx.end());
    value = std::stod(stringValue);
    return true;
}

bool Server::add_pending_trx(std::string trx, std::string signature) const {
    std::string sender, receiver;
    double value;
    Server::parse_trx(trx, sender, receiver, value);
    std::string id;
    auto isIdEqual = [&id](const auto& clientPair){ return (clientPair.first)->get_id() == id; };
    id = sender;
    auto iterSender = std::find_if(clients.begin(), clients.end(), isIdEqual);
    id = receiver;
    auto iterReceiver = std::find_if(clients.begin(), clients.end(), isIdEqual);
    if (iterSender == clients.end() || iterReceiver == clients.end()) return false;

    bool authentic = crypto::verifySignature(iterSender->first->get_publickey(), trx, signature);
    if (authentic && iterSender->second >= value) {
        pending_trxs.push_back(trx);
        return true;
    }
    return false;
}

size_t Server::mine() {
    std::string mempool{};
    for (const std::string& trx: pending_trxs) {
        mempool += trx;
    }

    std::string hash{};
    auto iterMiner = clients.begin();
    size_t nonce;
    while (hash.substr(0, 10).find("00") == std::string::npos) {
        for (auto iter=clients.begin(); iter!=clients.end(); ++iter) {
            nonce = iter->first->generate_nonce();
            hash = crypto::sha256(mempool + std::to_string(nonce));
            if (hash.substr(0, 10).find("00") == std::string::npos) {
                iterMiner = iter;
                break;
            }
        }
    }

    for (const std::string& trx : pending_trxs) {
        std::string sender, receiver;
        double value;
        Server::parse_trx(trx, sender, receiver, value);

        std::string id;
        auto isIdEqual = [&id](const auto& clientPair){ return (clientPair.first)->get_id() == id; };
        id = sender;
        auto iterSender = std::find_if(clients.begin(), clients.end(), isIdEqual);
        id = receiver;
        auto iterReceiver = std::find_if(clients.begin(), clients.end(), isIdEqual);

        clients[iterSender->first] = clients[iterSender->first] - value;
        clients[iterReceiver->first] = clients[iterReceiver->first] + value;
    }

    pending_trxs.clear();
    std::cout << "miner id : " << iterMiner->first->get_id() << std::endl;
    clients[iterMiner->first] = clients[iterMiner->first] + 6.25;
    return nonce;
}

void show_wallets(const Server& server)
{
	std::cout << std::string(20, '*') << std::endl;
 	for(const auto& client: server.clients)
 		std::cout << client.first->get_id() <<  " : "  << client.second << std::endl;
 	std::cout << std::string(20, '*') << std::endl;
}