//testing file

#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include "./ui/ui.h"
#include "./utils/linked_list/linked_list.h"


int main() {
    int y;
	int x;
	int c_y = 0;
	char message[5] = {0};
	int size = 5;
	size_t counter = 0;
	bool is_done = false;
	struct LINKED_LIST_context *messages;

	UI__init_screen(&x, &y, &messages);

	UI__print_message("world\0", 6, y, messages);
	UI__print_message("hello\0", 6, y, messages);
	UI__print_message("a\0", 2, y, messages);
	UI__print_message("g\0", 2, y, messages);
	UI__print_message("world\0", 6, y, messages);
	UI__print_message("hello\0", 6, y, messages);
	UI__print_message("a\0", 2, y, messages);
	UI__print_message("g\0", 2, y, messages);

	while (false == is_done) {
		UI__get_message(message, y, &is_done, &counter);
	}

	UI__destroy_screen(messages);


    return 0;
}
