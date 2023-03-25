#include "aggregator.h"
#include <algorithm>
#include <iostream>
#include <ranges>
#include "test_t.h"

namespace aggregator {
aggregator::aggregator(std::unordered_map<uint32_t, test_t> &_tests) {
    tests.reserve(tests.size());
    for (auto [id, test] : _tests) {
        tests.push_back(test);
    }
    mask.resize(tests.size(), true);
    displayed = tests.size();
    sort_tests(ID);
    summarize();
}
const std::vector<test_t> &aggregator::get_tests() {
    return this->tests;
}

void aggregator::sort_tests(SORT_OPTION sort_by) {
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

const std::map<TEST_RESULT, uint32_t> &aggregator::get_summary() {
    return summary;
}

void aggregator::filter_tests(std::string filter) {
    displayed = 0;
    for (size_t index = 0; index < tests.size(); ++index) {
        mask[index] = tests[index].contains(filter);
        displayed++;
    }
    summarize();
}
void aggregator::summarize() {
    summary.clear();
    for (size_t index = 0; index < tests.size(); ++index) {
        if (!summary.contains(tests[index].testResult)) {
            summary[tests[index].testResult] = 0;
        }
        if (mask[index]) {
            summary[tests[index].testResult]++;
        }
    }
}
}  // namespace aggregator