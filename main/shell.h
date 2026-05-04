#include "data.h"
#include <stdio.h>



#define TOP_LINE_Y 6
#define LINE_HEIGHT 8
#define LINE_WIDTH 25
#define BUFFER_SIZE 255
#define MIN_BUFFER_POS 14
#define PROMPT "dsmith@agsb $ "
#define MAX_CHARS_ON_SCREEN 190



// SHELL MODE
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


// VIEW MODE
enum mode_t {
	SHELL = 0,
	VIEW  = 1
} curr_mode = SHELL;
int curr_view_page, old_curr_view_page, curr_cmd_index;



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
	int input_len = 0;

	// clear buffer + transfer to input_buffer in 1 go
	for (int i = MIN_BUFFER_POS; buffer_pos > MIN_BUFFER_POS && buffer[i] != '\0'; i++, buffer_pos--) {
		input_buffer[i-MIN_BUFFER_POS] = buffer[i];
		input_len++;
		buffer[i] = '\0';
	}

	for (int i = 0; i < cmd_count; i++) {
		bool match = curr_mode != VIEW;
		const int cmd_name_len = cmd_name_lens[i];

		printf("CHECKING LENS: %d vs %d\n", cmd_name_len, input_len);
		if (cmd_name_len != input_len) continue;

		for (int j = 0; j < cmd_name_lens[i] && input_buffer[j] != '\0'; j++) {
			printf("COMPARISON NO %d: %c vs %c\n", j, input_buffer[j], cmd_names[i][j]);
			match = match && (cmd_names[i][j] == input_buffer[j]);
		}

		printf("SUBMIT: MATCH = %d\n", match);
		if (match) {
			curr_mode = VIEW; curr_cmd_index = i; curr_view_page = 0; old_curr_view_page = -1;
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

void handle_shell_input() {
    if (input != '\0' && input != old_input) {
		if (input == '*' && buffer_pos > MIN_BUFFER_POS) del();

		else if (input == '#') submit();

		else {
			// QOL - advance a space if user changes character
			if (buffer[buffer_pos] != '\0' && last_char != input) submit();
			add();
		}

		last_char = input;
	}

	old_input = input;
}

void mv_page(const int sign) {
	if (sign != 1 && sign != -1) return;
	if ((curr_view_page + sign) * MAX_CHARS_ON_SCREEN > cmd_lens[curr_cmd_index]
			|| (curr_view_page + sign) < 0) return;
	curr_view_page += sign;
}



void handle_view_input() {
	switch (input) {
		case '\0':
			return;
		case '#':
			mv_page(1);
			break;
		case '*':
			mv_page(-1);
			break;
		default: // Any other key will return to the shell
			curr_mode = SHELL;
			for (int i = 0; i < BUFFER_SIZE; i++) {
				if (i < MIN_BUFFER_POS) buffer[i] = PROMPT[i];
				else buffer[i] = '\0';
			}
			buffer_pos = MIN_BUFFER_POS;
            break;
	}

	// Do not modify buffer if nothing has changed!
	if (curr_view_page == old_curr_view_page) return;

	const char *cmd = cmds[curr_cmd_index];
	const int len = cmd_lens[curr_cmd_index];

	for (int i = 0; i < len; i++) {
		int index = curr_view_page * MAX_CHARS_ON_SCREEN + i;
		buffer[i] = index > len ? '\0' : cmd[index];
		printf("PRINTED %dth CHAR: %c\n", i, buffer[i]);
    }

    old_curr_view_page = curr_view_page;
}



void handle_input() { curr_mode == SHELL ? handle_shell_input() : handle_view_input(); }
