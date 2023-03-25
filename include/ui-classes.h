#include <ncurses.h>
#include <algorithm>
#include "aggregator.h"
#include "test_t.h"
#include "ui.h"

namespace aggregator {
struct SELECT {
private:
    std::vector<const char *> options = {
        "ID",
        "NAME",
        "DURATION",
        "RESULT"};
    int selected_option = 0;
    WINDOW *win;

public:
    SELECT(int, int, int, int);
    void draw();
    aggregator::SORT_OPTION select_sort();
};

struct TESTS_TABLE {
public:
    TESTS_TABLE(int nlines, int ncols, int begin_y, const std::vector<test_t> &_tests,
                const std::vector<bool> &_mask, const size_t &_displayed);

    void draw();
    void scrll(int);

private:
    int height;
    int width;

    const std::vector<test_t> &tests;

    const std::vector<bool> &mask;
    const size_t &displayed;
    WINDOW *win;
    int start_y = 0;

    void print_test_header(WINDOW *, int, int);
    void print_test(WINDOW *pad, int y, const test_t &t);
};
struct SUMMARY {
private:
    int height;
    int width;
    WINDOW *win;
    const std::map<TEST_RESULT, uint32_t> &summary;

public:
    SUMMARY(int, int, int, int, const std::map<TEST_RESULT, uint32_t> &);
    void draw();
};

struct FILTERS {
public:
    FILTERS(int, int, int, int);
    void draw();
    std::string get_filter();

private:
    int height;
    int width;
    WINDOW *win;
};

struct BOTTOM_MENU {
    BOTTOM_MENU();
};
}  // namespace aggregator