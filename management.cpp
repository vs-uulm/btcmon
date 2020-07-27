//
// Created by moedinger on 03.05.19.
//

#include "management.h"

uint32_t management::addTx(uint64_t time, const std::string &txId) {
    auto id = getId(txId);
    if(doneTx.find(id)!=doneTx.end())
        return 0;

    if(txMap.find(id)==txMap.end())
        txMap.emplace(id,std::vector<uint64_t>());
    txMap[id].push_back(time);
    return txMap[id].size();
}

std::vector<uint64_t> management::popTx(const std::string &txId) {
    auto id = getId(txId);
    if(txMap.find(id)==txMap.end())
        return std::vector<uint64_t>();
    auto tx = txMap[id];
    txMap.erase(id);
    doneTx.insert(id);
    return tx;
}

uint64_t management::getId(const std::string &txId) {
    return std::stoull(txId.substr(0,16),nullptr, 16);
}
