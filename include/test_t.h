#pragma once
#include <chrono>
#include <string>
namespace aggregator {
enum RESULT { SUCCESS,
              FAIL,
              UNKNOWN };

class test_t {
private:
    uint32_t id;
    std::string name;
    std::chrono::milliseconds duration;
    RESULT testResult;
    std::string errorText;

public:
    test_t() = default;
    test_t(uint32_t, std::string);
    [[maybe_unused]] test_t(uint32_t, std::chrono::milliseconds, RESULT, std::string);
    void update_on_finish(std::chrono::milliseconds, RESULT, std::string);
    friend std::ostream &operator<<(std::ostream &os, const test_t &test);
};
};  // namespace aggregator