#include <menu.h>

/*
The general flow of control of a menu program looks like this.

    1. Initialize curses

    2. Create items using new_item(). You can specify a name and description for the items.

    3. Create the menu with new_menu() by specifying the items to be attached with.

    4. Post the menu with menu_post() and refresh the screen.

    5. Process the user requests with a loop and do necessary updates to menu with menu_driver.

    6. Unpost the menu with menu_unpost()

    7. Free the memory allocated to menu by free_menu()

    8. Free the memory allocated to the items with free_item()

    9. End curses
*/

void init_gui() {
    printw("gui\n");
}

void terminate_gui() {}
