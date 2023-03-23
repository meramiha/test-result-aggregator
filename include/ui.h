#pragma once
#include <ncurses.h>
#include "aggregator.h"
#include "test_t.h"

namespace aggregator {
class ui {
public:
    ui(aggregator &);
    aggregator::SORT_OPTION select_sort();

private:
    void init();
    int32_t height;
    int32_t width;
    aggregator &a;
};
}  // namespace aggregator