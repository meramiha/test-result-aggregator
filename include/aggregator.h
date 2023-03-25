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
    const std::map<TEST_RESULT, uint32_t> &get_summary();

    enum SORT_OPTION {
        ID,
        NAME,
        DURATION,
        RESULT
    };

    void sort_tests(SORT_OPTION);
    void filter_tests(std::string);  // TODO
    size_t displayed;
    std::vector<bool> mask;

private:
    void summarize();

    std::vector<test_t> tests;
    std::map<TEST_RESULT, uint32_t> summary;
};
}  // namespace aggregator
