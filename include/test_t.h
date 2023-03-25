#pragma once
#include <chrono>
#include <string>
namespace aggregator {
enum TEST_RESULT { FAIL = 1,
                   SUCCESS = 2,
                   UNKNOWN = 4 };

class test_t {
public:
    uint32_t id;
    std::string name;
    std::chrono::milliseconds duration;
    TEST_RESULT testResult;
    std::string errorText;

public:
    test_t() = default;
    test_t(uint32_t, std::string);
    void update_on_finish(std::chrono::milliseconds, TEST_RESULT, std::string);
    bool contains(std::string);
    friend std::ostream &operator<<(std::ostream &os, const test_t &test);
};
}  // namespace aggregator