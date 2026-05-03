#pragma once



#define TOP_LINE_Y 6
#define LINE_HEIGHT 8
#define LINE_WIDTH 25
#define BUFFER_SIZE 255
#define MIN_BUFFER_POS 14
#define PROMPT "dsmith@agsb $ "



char input = '\0', old_input = 'Z';
char buffer[BUFFER_SIZE+1] = PROMPT;
int buffer_pos = MIN_BUFFER_POS; // convention: points to the next free position



void del() { buffer_pos--; buffer[buffer_pos] = '\0'; }

void submit() {
	while (buffer_pos > MIN_BUFFER_POS) {
		buffer_pos--;
		buffer[buffer_pos] = '\0';
	}
}

void add() {
	if (input != '#' && input != '*') {
		buffer[buffer_pos] = input;
		buffer_pos++;
	}
}



void handle_input() {
	if (input != '\0' && input != old_input) {
		if (input == '*' && buffer_pos > MIN_BUFFER_POS) del();

		else if (input == '#') submit();

		else add();
	}

	old_input = input;
}
