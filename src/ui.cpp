#include "ui.h"
#include <ncurses.h>
#include <algorithm>
#include "aggregator.h"
#include "ui-classes.h"

namespace aggregator {
void ui::init() {
    initscr();
    noecho();
    curs_set(0);
    scrollok(stdscr, false);

    start_color();
    use_default_colors();

    init_pair(1, COLOR_RED, -1);
    init_pair(2, COLOR_GREEN, -1);
    init_pair(4, COLOR_YELLOW, -1);
    init_pair(3, COLOR_CYAN, -1);

    getmaxyx(stdscr, height, width);
    if (freopen("/dev/tty", "r", stdin) == nullptr) {
        perror("/dev/tty");
        exit(1);
    }

    cbreak();
    keypad(stdscr, true);
}
ui::ui(aggregator &_a)
    : a(_a) {
    init();

    refresh();
    TESTS_TABLE tests_table(height - 10, width, 0, a.get_tests(), a.mask, a.displayed);
    tests_table.draw();

    SELECT select_menu(9, 20, height - 10, 0);
    select_menu.draw();

    FILTERS filters(9, 40, height - 10, 20);
    filters.draw();

    SUMMARY summary(9, 40, height - 10, width - 40, a.get_summary());
    summary.draw();

    refresh();

    BOTTOM_MENU{};

    int ch;
    while (true) {
        ch = getch();

        switch (ch) {
            case KEY_F(6): {
                a.sort_tests(select_menu.select_sort());

                tests_table.draw();

                refresh();
                doupdate();

                break;
            }
            case KEY_F(4): {
                a.filter_tests(filters.get_filter());
                tests_table.draw();
                summary.draw();
                refresh();

                break;
            }
            case 'u':
            case KEY_UP: {
                tests_table.scrll(-1);
                break;
            }
            case 'd':
            case KEY_DOWN: {
                tests_table.scrll(1);
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
}

}  // namespace aggregator