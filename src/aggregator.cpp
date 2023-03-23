#include "aggregator.h"
#include <algorithm>
#include <iostream>
#include <ranges>
#include "test_t.h"

namespace aggregator {
aggregator::aggregator(std::unordered_map<uint32_t, test_t> &tests) {
    this->tests.reserve(tests.size());
    for (auto [id, test] : tests) {
        this->tests.push_back(test);
    }
    sort_tests();
    summarize();
}
const std::vector<test_t> &aggregator::get_tests() {
    return this->tests;
}

void aggregator::sort_tests() {
    std::unordered_map<SORT_OPTION, bool (*)(const test_t &, const test_t &)> comparator = {
        {ID, [](const test_t &a, const test_t &b) {
             return a.id < b.id;
         }},
        {NAME, [](const test_t &a, const test_t &b) {
             return a.name < b.name;
         }},
        {DURATION, [](const test_t &a, const test_t &b) {
             return a.duration < b.duration;
         }},
        {RESULT, [](const test_t &a, const test_t &b) {
             return a.testResult < b.testResult;
         }}};
    std::stable_sort(tests.begin(), tests.end(), comparator[sort_by]);
}
std::map<TEST_RESULT, uint32_t> &aggregator::get_summary() {
    return summary;
}
void aggregator::summarize() {
    for (const auto &test : tests) {
        if (!summary.contains(test.testResult))
            summary[test.testResult] = 0;
        summary[test.testResult]++;
    }
}
}  // namespace aggregator