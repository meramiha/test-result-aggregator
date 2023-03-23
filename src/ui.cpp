#include "ui.h"
#include <ncurses.h>
#include <algorithm>
#include <iostream>
#include "aggregator.h"
#include "test_t.h"

namespace aggregator {
static std::unordered_map<TEST_RESULT, std::string> table = {
    {SUCCESS, "SUCCESS"},
    {FAIL, "FAIL"},
    {UNKNOWN, "UNKNOWN"},
};

aggregator::SORT_OPTION ui::select_sort() {
    WINDOW *select_menu = newwin(9, 20, height - 10, 0);
    box(select_menu, 0, 0);
    mvwprintw(select_menu, 0, 1, "Sort by");
    wrefresh(select_menu);

    int selected_option = 0;

    std::vector<const char *> options = {
        "ID",
        "NAME",
        "DURATION",
        "RESULT"};

    while (true) {
        for (size_t i = 0; i < options.size(); ++i) {
            if (i == selected_option) {
                wattron(select_menu, A_REVERSE);
            }
            mvwprintw(select_menu, i + 1, 1, options[i]);
            if (i == selected_option) {
                wattroff(select_menu, A_REVERSE);
            }
            wrefresh(select_menu);
        }
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
                werase(select_menu);
                delwin(select_menu);
                return static_cast<aggregator::SORT_OPTION>(selected_option);
            }
            default:
                break;
        }
    }
}
void ui::init() {
    initscr();
    noecho();

    start_color();
    use_default_colors();

    init_pair(1, COLOR_RED, -1);
    init_pair(2, COLOR_GREEN, -1);
    init_pair(4, COLOR_YELLOW, -1);
    init_pair(3, -1, COLOR_CYAN);

    getmaxyx(stdscr, height, width);
    if (freopen("/dev/tty", "r", stdin) == nullptr) {
        perror("/dev/tty");
        exit(1);
    }

    cbreak();
    keypad(stdscr, true);
}
struct TESTS_TABLE {
private:
    int height;
    int width;
    WINDOW *win;
    const std::vector<test_t> &tests;

public:
    TESTS_TABLE(int nlines, int ncols, int begin_y, int begin_x, const std::vector<test_t> &_tests)
        : height(nlines), width(ncols), win(newwin(nlines, ncols, begin_y, begin_x)), tests(_tests) {
    }

    void draw() {
        wclear(win);
        box(win, 0, 0);
        mvwprintw(win, 0, 1, "Tests");
        int y = 1;
        int x = 2;

        print_test_header(y++, x);
        for (const auto &test : tests) {
            print_test(y++, x, test);
        }
        wrefresh(win);
    }

private:
    void print_test_header(int y, int x) {
        wattrset(win, COLOR_PAIR(3));
        for (int i = 1; i < width - 1; ++i) {
            mvwprintw(win, y, i, "%s", " ");
        }

        mvwprintw(win, y, x + 0, "%-6s", "ID");
        mvwprintw(win, y, x + 6, "%-20s", "NAME");
        mvwprintw(win, y, x + 26, "%-12s", "DURATION");
        mvwprintw(win, y, x + 38, "%-15s", "RESULT");
        mvwprintw(win, y, x + 53, "%-20s", "ERROR");
        wattrset(win, A_NORMAL);
    }
    void print_test(int y, int x, const test_t &t) {
        if (x + 28 > width)
            return;
        mvwprintw(win, y, x + 0, "%-6d", t.id);
        mvwprintw(win, y, x + 6, "%-20s", t.name.c_str());
        if (t.testResult != UNKNOWN) {
            mvwprintw(win, y, x + 26, "%-12d", t.duration);
        }
        switch (t.testResult) {
            case FAIL: {
                wattrset(win, COLOR_PAIR(1) | A_BOLD);
                break;
            }
            case SUCCESS: {
                wattrset(win, COLOR_PAIR(2) | A_BOLD);
                break;
            }
            case UNKNOWN: {
                wattrset(win, COLOR_PAIR(4) | A_BOLD);
            }
        }
        mvwprintw(win, y, x + 38, "%-15s", table[t.testResult].c_str());
        wattrset(win, A_NORMAL);

        if (!t.errorText.empty())
            mvwprintw(win, y, x + 53, "%-20s", t.errorText.c_str());
    }
};
struct SUMMARY {
private:
    int height;
    int width;
    WINDOW *win;
    std::map<TEST_RESULT, uint32_t> &summary;

public:
    SUMMARY(int nlines, int ncols, int begin_y, int begin_x, std::map<TEST_RESULT, uint32_t> &_summary)
        : height(nlines), width(ncols), win(newwin(nlines, ncols, begin_y, begin_x)), summary(_summary) {
    }
    void draw() {
        wclear(win);
        box(win, 0, 0);
        mvwprintw(win, 0, 1, "Summary");

        int y = 1;
        for (auto [result_type, count] : summary) {
            wattron(win, COLOR_PAIR(static_cast<int>(result_type)) | A_BOLD);
            mvwprintw(win, y++, 1, "%s:%-10d", table[result_type].c_str(), count);
            wattroff(win, COLOR_PAIR(static_cast<int>(result_type)) | A_BOLD);
        }
        wrefresh(win);
    }
};
ui::ui(aggregator &_a)
    : a(_a) {
    init();

    refresh();
    TESTS_TABLE tests_table(height - 10, width, 0, 0, a.get_tests());
    tests_table.draw();

    WINDOW *filters = newwin(9, width - 40, height - 10, 0);
    box(filters, 0, 0);
    mvwprintw(filters, 0, 1, "Filters");

    SUMMARY summary(9, 40, height - 10, width - 40, a.get_summary());
    summary.draw();

    refresh();
    wrefresh(filters);

    int ch;
    while (true) {
        ch = getch();
        mvaddch(height - 1, 1, ch);
        switch (ch) {
            case KEY_F(6): {
                mvprintw(height - 1, 1, "F6 pressed");
                a.sort_by = select_sort();
                a.sort_tests();
                tests_table.draw();

                refresh();
                doupdate();

                break;
            }
            case 'q':
            case KEY_F(10): {
                endwin();
                exit(0);
            }

            default:
                break;
        }
    }

    endwin();
}

}  // namespace aggregator