#include "aggregator.h"
#include <iostream>
#include <ranges>
#include "test_t.h"

namespace aggregator {
aggregator::aggregator(std::unordered_map<uint32_t, test_t> &tests) {
    this->tests.reserve(tests.size());
    for (auto [id, test] : tests) {
        this->tests.push_back(test);
    }
}
// std::ranges::filter_view<> aggregator::filter() {
//     auto k = this->tests | std::views::filter([](const test_t &test) {
//                  return test.testResult == 0;
//              });
//     return k;
// }
}  // namespace aggregator