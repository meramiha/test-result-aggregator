#pragma once
#include <map>
#include <unordered_map>  // TODO change to map
#include <vector>
#include "test_t.h"

namespace aggregator {
class aggregator {
public:
    aggregator(std::unordered_map<uint32_t, test_t> &);
    aggregator(std::unordered_map<uint32_t, test_t> &&);
    const std::vector<test_t> &get_tests();
    std::map<TEST_RESULT, uint32_t> &get_summary();

    enum SORT_OPTION {
        ID,
        NAME,
        DURATION,
        RESULT
    };

    SORT_OPTION sort_by = ID;
    void sort_tests();

private:
    void summarize();
    std::vector<test_t> tests;
    std::map<TEST_RESULT, uint32_t> summary;
};
}  // namespace aggregator
