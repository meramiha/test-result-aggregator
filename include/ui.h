#pragma once
#include <ncurses.h>
#include "aggregator.h"
#include "test_t.h"

namespace aggregator {
class ui {
private:
    aggregator &a;
    ui();
};
}  // namespace aggregator