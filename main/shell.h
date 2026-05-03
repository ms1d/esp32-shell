#pragma once



#include "data.h"
#include <stdio.h>



#define TOP_LINE_Y 6
#define LINE_HEIGHT 8
#define LINE_WIDTH 25
#define BUFFER_SIZE 255
#define MIN_BUFFER_POS 14
#define PROMPT "dsmith@agsb $ "



// input = current input THIS cycle
// old_input = last input LAST cycle
// last_char = last key pressed (i.e. last valid input)
char input = '\0', old_input = 'Z', last_char = 'X';
char buffer[BUFFER_SIZE+1] = PROMPT;
int buffer_pos = MIN_BUFFER_POS; // convention: points to the next free position
char letters[10][4] = {
	{ '\0', '\0', '\0', '\0' },	 // 0
	{ '\0', '\0', '\0', '\0' },	 // 1
	{ 'a' , 'b' , 'c' , '\0' },  // 2
	{ 'd' , 'e' , 'f' , '\0' },  // 3
	{ 'g' , 'h' , 'i' , '\0' },  // 4
	{ 'j' , 'k' , 'l' , '\0' },  // 5
	{ 'm' , 'n' , 'o' , '\0' },  // 6
	{ 'p' , 'q' , 'r' , 's'  },  // 7
	{ 't' , 'u' , 'v' , '\0' },  // 8
	{ 'w' , 'x' , 'y' , 'z'  }   // 9
};


void del() {
	if (buffer[buffer_pos] != '\0') buffer[buffer_pos] = '\0';
	else {
		buffer_pos--;
		buffer[buffer_pos] = '\0';
	}
}

void submit() {
	if (buffer[buffer_pos] != '\0') {
		buffer_pos++;
		return;
	}

	char input_buffer[BUFFER_SIZE+1];

	// clear buffer + transfer to input_buffer in 1 go
	for (int i = MIN_BUFFER_POS; buffer_pos > MIN_BUFFER_POS; i++, buffer_pos--) {
		input_buffer[i-MIN_BUFFER_POS] = buffer[i];
		buffer[i] = '\0';
	}

	for (int i = 0; i < cmd_count; i++) {
		if (cmd_names[i] == input_buffer) {
			for (int j = 0; j < BUFFER_SIZE; j++) buffer[j] = input_buffer[j];
			break;
		}
	}
}

void add() {
	static int curr_cycle = 0;

	if (input == '#' || input == '*') return;

	curr_cycle = input == last_char ? (curr_cycle + 1) % 4: 0;

	char char_to_push = letters[(int)input - '0'][curr_cycle];
	if (char_to_push == '\0') return;

	buffer[buffer_pos] = char_to_push;
	buffer_pos = input == last_char
		|| (input != last_char && old_input == '\0')
	   	? buffer_pos : buffer_pos + 1;
	printf("BREAK: BUFFER = %s\n", buffer);
	printf("CHAR PUSHED = %c\n", char_to_push);
}



void handle_input() {
	if (input != '\0' && input != old_input) {
		if (input == '*' && buffer_pos > MIN_BUFFER_POS) del();

		else if (input == '#') submit();

		else {
			// QOL - advance a space if user changes character
			if (last_char != input) submit();
			add();
		}

		last_char = input;
	}

	old_input = input;
}
