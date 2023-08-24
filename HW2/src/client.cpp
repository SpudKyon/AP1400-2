#include "client.h"
#include "server.h"
#include "crypto.h"

Client::Client(std::string id, const Server &server) : server(&server), id(id) {
    crypto::generate_key(this->public_key, this->private_key);
}

std::string Client::get_id() const {
    return this->id;
}

double Client::get_wallet() {
    return this->server->get_wallet(this->id);
}

std::string Client::get_publickey() const {
    return this->public_key;
}

std::string Client::sign(std::string txt) const {
    return crypto::signMessage(private_key, txt);
}

bool Client::transfer_money(std::string receiver, double value) {
    std::string trx = this->id + "-" + receiver + "-" + std::to_string(value);
    std::string signature = crypto::signMessage(this->private_key, trx);
    return this->server->add_pending_trx(trx, signature);
}

size_t Client::generate_nonce() {
    std::random_device dev;
    std::uniform_int_distribution<int> dis(0, INT_MAX);
    std::mt19937 rng(dev());
    return dis(rng);
}