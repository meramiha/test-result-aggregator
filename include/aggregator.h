#pragma once
#include <unordered_map>
#include <vector>
#include "test_t.h"

namespace aggregator {
class aggregator {
public:
    aggregator(std::unordered_map<uint32_t, test_t> &);
    aggregator(std::unordered_map<uint32_t, test_t> &&);
    // int filter()
private:
    std::vector<test_t> tests;
    uint32_t success = 0;
    uint32_t fail = 0;
    uint32_t other = 0;
};
}  // namespace aggregator
