import cmd
import curses

def display_settings_menu():
    curses.wrapper(settings_menu)

def settings_menu(stdscr):
    # Initialize color pair for selected menu item
    curses.start_color()
    curses.init_pair(1, curses.COLOR_BLACK, curses.COLOR_WHITE)

    # Clear screen
    stdscr.clear()
    stdscr.refresh()

    # Menu options
    menu = ['Option 1', 'Option 2', 'Option 3', 'Exit']
    current_row = 0

    while True:
        stdscr.clear()
        h, w = stdscr.getmaxyx()

        for idx, row in enumerate(menu):
            x = w // 2 - len(row) // 2
            y = h // 2 - len(menu) // 2 + idx
            if idx == current_row:
                stdscr.attron(curses.color_pair(1))
                stdscr.addstr(y, x, row)
                stdscr.attroff(curses.color_pair(1))
            else:
                stdscr.addstr(y, x, row)

        stdscr.refresh()

        key = stdscr.getch()

        # Debug print (comment out in production)
        stdscr.addstr(0, 0, f"Key pressed: {key}")
        stdscr.refresh()

        if key in [curses.KEY_UP, ord('w')] and current_row > 0:
            current_row -= 1
        elif key in [curses.KEY_DOWN, ord('s')] and current_row < len(menu) - 1:
            current_row += 1
        elif key in [curses.KEY_ENTER, ord('\n'), ord('\r')]:
            if menu[current_row] == 'Exit':
                break

class BIOSCmd(cmd.Cmd):
    prompt = '> '

    def do_exit(self, inp):
        '''Exit the BIOS settings UI.'''
  

