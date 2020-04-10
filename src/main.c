//testing file

#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include "./ui/ui.h"


int main() {
    	int y;
	int x;
	int c_y = 0;
	char message[5] = {0};
	int size = 5;
	size_t counter = 0;
	bool is_done = false;

	printf("starts!\n");

	UI__init_screen(&x, &y);

	/*
	for (int i = 0; i < 10; i++) {
		UI__print_message("world", 5, y, &c_y);
	}
	*/

	while (false == is_done) {
		UI__get_message(message, y, &is_done, &counter);
	}


	UI__destroy_screen();


    return 0;
}
