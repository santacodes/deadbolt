import curses
import time


def generate_totp():
    return "123456"  # Dummy TOTP, replace with C bindings


def draw_ui(stdscr, entries, selected, input_mode, input_text, secret_input):
    stdscr.clear()
    height, width = stdscr.getmaxyx()
    list_width = width // 2
    input_width = width - list_width - 4

    # Draw border
    stdscr.border()

    # Centering calculations
    start_y = (height - len(entries) - 6) // 2
    start_x = (list_width - 20) // 2
    input_x = list_width + 4

    # Draw TOTP entries
    stdscr.addstr(start_y - 2, start_x, "TOTP Entries", curses.A_BOLD)
    for i, (service, totp) in enumerate(entries.items()):
        prefix = "> " if i == selected else "  "
        stdscr.addstr(start_y + i, start_x, f"{prefix}{service}: {totp}")

    # Draw input area
    stdscr.addstr(start_y - 2, input_x, "Manage TOTP", curses.A_BOLD)
    if input_mode == "add":
        stdscr.addstr(start_y, input_x, "[Adding] Enter Service:")
        stdscr.addstr(start_y + 1, input_x, input_text)
        stdscr.addstr(start_y + 3, input_x, "Enter Secret:")
        stdscr.addstr(start_y + 4, input_x, secret_input)
    elif input_mode == "delete":
        stdscr.addstr(start_y, input_x, "[Deleting] Confirm: (y/n)")

    # Draw keymap at bottom
    stdscr.addstr(
        height - 2,
        (width - 50) // 2,
        "[j] Down  [k] Up  [a] Add  [d] Delete  [q] Quit",
        curses.A_REVERSE,
    )

    stdscr.refresh()


def main(stdscr):
    curses.curs_set(0)
    stdscr.nodelay(1)
    stdscr.timeout(1000)

    entries = {"GitHub": generate_totp(), "Google": generate_totp()}
    selected = 0
    input_mode = None
    input_text = ""
    secret_input = ""
    entering_secret = False

    while True:
        draw_ui(stdscr, entries, selected, input_mode, input_text, secret_input)
        key = stdscr.getch()

        if input_mode is None:  # Key mappings only active outside input mode
            if key == ord("q"):
                break
            elif key == ord("j") and selected < len(entries) - 1:
                selected += 1
            elif key == ord("k") and selected > 0:
                selected -= 1
            elif key == ord("a"):
                input_mode = "add"
                input_text = ""
                secret_input = ""
                entering_secret = False
            elif key == ord("d") and entries:
                input_mode = "delete"

        elif input_mode == "add":
            if not entering_secret:
                if key == 10:  # Enter moves to secret input
                    entering_secret = True
                elif key == 27:  # Escape cancels
                    input_mode = None
                elif key == curses.KEY_BACKSPACE or key == 127:
                    input_text = input_text[:-1]
                elif key >= 32 and key <= 126:
                    input_text += chr(key)
            else:
                if key == 10:  # Enter confirms and adds TOTP
                    if input_text and secret_input:
                        entries[input_text] = generate_totp()
                    input_mode = None
                elif key == 27:  # Escape cancels
                    input_mode = None
                elif key == curses.KEY_BACKSPACE or key == 127:
                    secret_input = secret_input[:-1]
                elif key >= 32 and key <= 126:
                    secret_input += chr(key)
        elif input_mode == "delete":
            if key == ord("y") and entries:
                service = list(entries.keys())[selected]
                del entries[service]
                selected = max(0, selected - 1)
            input_mode = None

        # Auto-refresh TOTP every 30s
        if time.time() % 30 < 1:
            for service in entries:
                entries[service] = generate_totp()


curses.wrapper(main)
