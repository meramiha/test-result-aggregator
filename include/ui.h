#pragma once
#include <ncurses.h>
#include "aggregator.h"
#include "test_t.h"

namespace aggregator {
class ui {
public:
    ui(aggregator &);
    void print_test(WINDOW *win, int y, int x, const test_t &t);
    void print_test_header(WINDOW *win, int y, int x);

private:
    uint32_t height;
    uint32_t width;
    aggregator &a;
};
}  // namespace aggregator