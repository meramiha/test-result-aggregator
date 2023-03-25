#include "ui-classes.h"
#include <ncurses.h>
#include <algorithm>
#include <iostream>
#include "aggregator.h"
#include "test_t.h"
#include "ui.h"

namespace aggregator {
static std::unordered_map<TEST_RESULT, std::string> table = {
    {SUCCESS, "SUCCESS"},
    {FAIL, "FAIL"},
    {UNKNOWN, "UNKNOWN"},
};

SELECT::SELECT(int nlines, int ncols, int begin_y, int begin_x)
    : win(newwin(nlines, ncols, begin_y, begin_x)) {
}
void SELECT::draw() {
    box(win, 0, 0);
    mvwprintw(win, 0, 1, "Sort by");

    for (size_t i = 0; i < options.size(); ++i) {
        if (i == selected_option) {
            wattron(win, A_REVERSE | COLOR_PAIR(3));
        }
        mvwprintw(win, i + 1, 1, options[i]);
        if (i == selected_option) {
            wattroff(win, A_REVERSE | COLOR_PAIR(3));
        }
    }

    wrefresh(win);
}
aggregator::SORT_OPTION SELECT::select_sort() {
    while (true) {
        draw();
        int ch = getch();
        switch (ch) {
            case KEY_UP: {
                selected_option = (selected_option - 1 + options.size()) % options.size();
                break;
            }
            case KEY_DOWN: {
                selected_option = (selected_option + 1) % options.size();
                break;
            }
            case 10:
            case KEY_ENTER: {
                return static_cast<aggregator::SORT_OPTION>(selected_option);
            }
            default:
                break;
        }
    }
}

TESTS_TABLE::TESTS_TABLE(int nlines,
                         int ncols,
                         int begin_y,
                         int begin_x,
                         const std::vector<test_t> &_tests,
                         const std::vector<bool> &_mask,
                         const size_t &_displayed)
    : height(nlines),
      width(ncols),
      tests(_tests),
      mask(_mask),
      displayed(_displayed),
      win(newpad(tests.size(), width)) {
    print_test_header(stdscr, 0, 0);
}

void TESTS_TABLE::draw() {
    start_y = 0;
    wclear(win);

    int y = 0;
    int x = 2;

    for (size_t index = 0; index < tests.size(); ++index) {
        if (mask[index]) {
            print_test(win, y++, 0, tests[index]);
        }
    }

    refresh();
    prefresh(win, start_y, 0, 1, 0, height - 1, COLS);
}
void TESTS_TABLE::scrll(int shift) {
    if (start_y + shift > 0 && start_y + shift < (int)displayed - height + 1) {
        start_y += shift;
        prefresh(win, start_y, 0, 1, 0, height - 1, COLS);
    }
}

void TESTS_TABLE::print_test_header(WINDOW *win, int y, int x) {
    wattrset(win, COLOR_PAIR(3) | A_REVERSE);
    for (int i = 1; i < width; ++i) {
        mvwprintw(win, y, i, "%s", " ");
    }

    mvwprintw(win, y, x + 0, "%-6s", "ID");
    mvwprintw(win, y, x + 6, "%-20s", "NAME");
    mvwprintw(win, y, x + 26, "%-12s", "DURATION");
    mvwprintw(win, y, x + 38, "%-15s", "RESULT");
    mvwprintw(win, y, x + 53, "%-20s", "ERROR");
    wattrset(win, A_NORMAL);
}
void TESTS_TABLE::print_test(WINDOW *pad, int y, int x, const test_t &t) {
    if (x + 28 > width)
        return;
    mvwprintw(pad, y, x + 0, "%-6d", t.id);
    mvwprintw(pad, y, x + 6, "%-20s", t.name.c_str());
    if (t.testResult != UNKNOWN) {
        mvwprintw(pad, y, x + 26, "%-12d", t.duration);
    }
    switch (t.testResult) {
        case FAIL: {
            wattrset(pad, COLOR_PAIR(1) | A_BOLD);
            break;
        }
        case SUCCESS: {
            wattrset(pad, COLOR_PAIR(2) | A_BOLD);
            break;
        }
        case UNKNOWN: {
            wattrset(pad, COLOR_PAIR(4) | A_BOLD);
        }
    }
    mvwprintw(pad, y, x + 38, "%-15s", table[t.testResult].c_str());
    wattrset(pad, A_NORMAL);

    if (!t.errorText.empty())
        mvwprintw(pad, y, x + 53, "%-20s", t.errorText.c_str());
}

SUMMARY::SUMMARY(int nlines,
                 int ncols,
                 int begin_y,
                 int begin_x,
                 const std::map<TEST_RESULT, uint32_t> &_summary)
    : height(nlines),
      width(ncols),
      win(newwin(nlines, ncols, begin_y, begin_x)),
      summary(_summary) {
}
void SUMMARY::draw() {
    wclear(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 1, "Summary");

    int y = 2;
    size_t sum = 0;
    for (auto [result_type, count] : summary) {
        sum += count;
        wattron(win, COLOR_PAIR(static_cast<int>(result_type)) | A_BOLD);
        mvwprintw(win, y++, 1, "%s:%-10d", table[result_type].c_str(), count);
        wattroff(win, COLOR_PAIR(static_cast<int>(result_type)) | A_BOLD);
    }

    mvwprintw(win, 1, 1, "%s:%-10d", "TOTAL", sum);

    wrefresh(win);
}

FILTERS::FILTERS(int nlines, int ncols, int begin_y, int begin_x)
    : height(nlines), width(ncols), win(newwin(nlines, ncols, begin_y, begin_x)) {
}
void FILTERS::draw() {
    wclear(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 1, "Filters");
    wattron(win, A_REVERSE);
    mvwprintw(win, 1, 1, "YOUR FILTER:");
    wattroff(win, A_REVERSE);
    wrefresh(win);
}
std::string FILTERS::get_filter() {
    draw();
    int y = 2;
    int x = 1;

    std::string filter;
    while (true) {
        int ch = getch();
        curs_set(1);
        switch (ch) {
            case 27:
            case KEY_EXIT: {
                draw();
                curs_set(0);
                return "";
            }
            case 10:
            case KEY_ENTER: {
                curs_set(0);
                return filter;
            }
            case 8:
            case 127: {
                if (!filter.empty()) {
                    mvwaddch(win, y, --x, ' ');
                    wmove(win, y, x);
                    filter.pop_back();
                }
                wrefresh(win);
                break;
            }

            default: {
                if (std::isprint(ch) && x < width - 1) {
                    mvwaddch(win, y, x++, ch);
                    filter.push_back(ch);
                }
                wrefresh(win);
                break;
            }
        }
    }
}

BOTTOM_MENU::BOTTOM_MENU() {
    mvprintw(LINES - 1, 5, "F4");
    attrset(COLOR_PAIR(3) | A_REVERSE);
    mvprintw(LINES - 1, 7, "Filter");
    attrset(A_NORMAL);

    mvprintw(LINES - 1, 13, "F6");
    attrset(COLOR_PAIR(3) | A_REVERSE);
    mvprintw(LINES - 1, 15, "Sort");
    attrset(A_NORMAL);

    mvprintw(LINES - 1, 19, "F10");
    attrset(COLOR_PAIR(3) | A_REVERSE);
    mvprintw(LINES - 1, 22, "Exit");
    attrset(A_NORMAL);
}
};  // namespace aggregator