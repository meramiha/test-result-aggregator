#include "ui.h"
#include <ncurses.h>
#include "aggregator.h"
#include "test_t.h"

namespace aggregator {

void ui::print_test_header(WINDOW *win, int y, int x) {
    wattrset(win, COLOR_PAIR(3));
    for (int i = 1; i < width - 1; ++i) {
        mvwprintw(win, y, i, "%s", " ");
    }

    mvwprintw(win, y, x + 0, "%-6s", "ID");
    mvwprintw(win, y, x + 6, "%-10s", "NAME");
    mvwprintw(win, y, x + 16, "%-12s", "DURATION");
    mvwprintw(win, y, x + 28, "%-15s", "RESULT");
    mvwprintw(win, y, x + 43, "%-20s", "ERROR");
    wattrset(win, A_NORMAL);
}
void ui::print_test(WINDOW *win, int y, int x, const test_t &t) {
    static std::unordered_map<RESULT, std::string> table = {
        {SUCCESS, "SUCCESS"},
        {FAIL, "FAIL"},
        {UNKNOWN, "UNKNOWN"},
    };

    if (x + 28 > width)
        return;
    mvwprintw(win, y, x + 0, "%-6d", t.id);
    mvwprintw(win, y, x + 6, "%-10s", t.name.c_str());
    mvwprintw(win, y, x + 16, "%-12d", t.duration);
    switch (t.testResult) {
        case FAIL: {
            wattrset(win, COLOR_PAIR(1) | A_BOLD);
            break;
        }
        case SUCCESS: {
            wattrset(win, COLOR_PAIR(2) | A_BOLD);
            break;
        }
    }
    mvwprintw(win, y, x + 28, "%-15s", table[t.testResult].c_str());
    wattrset(win, A_NORMAL);

    if (!t.errorText.empty())
        mvwprintw(win, y, x + 43, "%-20s", t.errorText.c_str());
}

ui::ui(aggregator &_a)
    : a(_a) {
    // INIT
    initscr();
    noecho();

    start_color();
    use_default_colors();

    init_pair(1, COLOR_RED, -1);
    init_pair(2, COLOR_GREEN, -1);
    init_pair(3, -1, COLOR_CYAN);
    getmaxyx(stdscr, height, width);
    if (!freopen("/dev/tty", "r", stdin)) {
        perror("/dev/tty");
        exit(1);
    }
    cbreak();
    keypad(stdscr, true);
    // INIT

    WINDOW *tests_table = newwin(height - 10, width, 0, 0);
    box(tests_table, 0, 0);
    mvwprintw(tests_table, 0, 1, "Tests");

    WINDOW *filters = newwin(9, width - 40, height - 10, 0);
    box(filters, 0, 0);
    mvwprintw(filters, 0, 1, "Filters");

    WINDOW *summery = newwin(9, 40, height - 10, width - 40);
    box(summery, 0, 0);
    mvwprintw(summery, 0, 1, "Summery");

    int y = 1, x = 2;

    print_test_header(tests_table, y++, x);
    for (auto test : a.get_tests())
        print_test(tests_table, y++, x, test);

    refresh();
    wrefresh(tests_table);
    wrefresh(filters);
    wrefresh(summery);

    int mypadpos = 0;
    int ch;
    while (ch = wgetch(win1); ch != 'q' || ch != KEY_F(10)) {
        switch (ch) {
            case KEY_UP:
                if (mypadpos >= 0)
                    mypadpos--;

                prefresh(win1, mypadpos, 0, 0, 0, maxy, maxx);
                break;

            case KEY_DOWN:
                if (mypadpos <= rowcount + 1)
                    mypadpos++;

                prefresh(win1, mypadpos, 0, 0, 0, maxy - 1, maxx);
                break;
        }
    }

    endwin();
}

}  // namespace aggregator