//
// Created by moedinger on 03.05.19.
//

#ifndef BTCMON_MANAGEMENT_H
#define BTCMON_MANAGEMENT_H


#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

class management {
private:
    std::unordered_map<uint64_t,std::vector<uint64_t> > txMap;
    std::set<uint64_t> doneTx;

    uint64_t getId(const std::string& txId);
public:

    uint32_t addTx(uint64_t time, const std::string& txId);
    std::vector<uint64_t> popTx(const std::string& txId);
};

#endif //BTCMON_MANAGEMENT_H
