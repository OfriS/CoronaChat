//testing file

#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include "./ui/ui.h"


int main() {
    	int y;
	int x;
	int c_y = 0;
	char message[5] = "hello";
	int size = 5;

	printf("starts!\n");

	sleep(1);

	UI__init_screen(&x, &y);

	for (int i = 0; i < 10; i++) {
		UI__print_message(message, 5, y, &c_y);
		UI__print_message("world", 5, y, &c_y);
	}

	UI__get_message(message,(size_t *) &size, y);

	sleep(2);

	UI__destroy_screen();


    return 0;
}
