#include "server.h"

std::vector<std::string> pending_trxs;

void show_pending_transactions() {
    std::cout << std::string(20, '*') << std::endl;
    for (const auto &trx: pending_trxs)
        std::cout << trx << std::endl;
    std::cout << std::string(20, '*') << std::endl;
}

void show_wallets(const Server &server) {
    std::cout << std::string(20, '*') << std::endl;
    for (const auto &client: server.clients)
        std::cout << client.first->get_id() << " : " << client.second << std::endl;

    std::cout << std::string(20, '*') << std::endl;
}

std::shared_ptr<Client> Server::add_client(std::string id) {
    for (auto &client: clients) {
        if (client.first->get_id() == id) {
            std::random_device dev;
            std::uniform_int_distribution<int> dis(1000, 9999);
            std::mt19937 rng(dev());
            id += std::to_string(dis(rng));
        }
    }

    Client result(id, *this);
    auto pclient = std::make_shared<Client>(result);
    clients.insert({pclient, 5.0});
    return pclient;
}

std::shared_ptr<Client> Server::get_client(std::string id) const {
    for (const auto &client: clients) {
        if (client.first->get_id() == id) {
            return client.first;
        }
    }
    return nullptr;
}

double Server::get_wallet(std::string id) const {
    for (const auto &client: clients) {
        if (client.first->get_id() == id) {
            return client.second;
        }
    }
    throw std::runtime_error("client of input id isn't exist.");
}

bool Server::parse_trx(std::string trx, std::string &sender, std::string &receiver, double &value) {
    std::stringstream sstr(trx);
    std::string temp;
    std::vector<std::string> out;
    while (std::getline(sstr, temp, '-')) {
        out.push_back(temp);
    }
    if (out.size() != 3) {
        throw std::runtime_error("the string's format is incorrect");
    }
    sender = out[0];
    receiver = out[1];
    value = stod(out[2]);
    return true;
}

bool Server::add_pending_trx(std::string trx, std::string signature) const {
    std::string sender{}, receiver{};
    double value;
    parse_trx(trx, sender, receiver, value);
    std::shared_ptr<Client> sender_client = get_client(sender);
    std::shared_ptr<Client> receiver_client = get_client(receiver);
    if (sender_client == nullptr || receiver_client == nullptr || sender_client == receiver_client) {
        return false;
    }
    if (this->get_wallet(sender) < value) {
        return false;
    }
    auto public_key = sender_client->get_publickey();
    bool authentic = crypto::verifySignature(public_key, trx, signature);
    if (authentic) {
        pending_trxs.push_back(trx);
        return true;
    }
    return false;
}

Server::Server() {
    pending_trxs.clear();
}

size_t Server::mine() {
    std::string mempool{};
    for (const auto trx: pending_trxs) {
        mempool += trx;
    }
    bool mined = false;
    std::shared_ptr<Client> miner;
    size_t nonce = 0;
    while (!mined) {
        for (auto client: clients) {
            nonce = client.first->generate_nonce();
            auto tempstr = mempool + std::to_string(nonce);
            std::string hash{crypto::sha256(tempstr)};
            if (hash.substr(0, 10).find("000") != std::string::npos) {

                mined = true;
                miner = client.first;
                break;
            }
        }
    }
    clients[miner] += 6.25;
    for (auto trx: pending_trxs) {
        std::string sender{}, receiver{};
        double value;
        parse_trx(trx, sender, receiver, value);

        auto client1 = get_client(sender);
        auto client2 = get_client(receiver);
        clients[client1] -= value;
        clients[client2] += value;
    }
    return nonce;
}
