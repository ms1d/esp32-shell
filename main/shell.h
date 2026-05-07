#include "data.h"
#include "sh1106_setup.h"



// Unix-like shell with input inspired by old phone keypads



#define TOP_LINE_Y 6
#define LINE_HEIGHT 8
#define LINE_WIDTH 25
#define SHELL_BUFFER_SIZE 1200
#define MIN_SHELL_BUFFER_POS 14
#define PROMPT "dsmith@agsb $ "
#define MAX_CHARS_ON_SCREEN 190
#define CURSOR "_"
#define DISPLAY_CHAR_WIDTH 5
#define SPACE_WIDTH 1
#define CURSOR_VERT_OFFSET 2



// SHELL MODE
// input = current input THIS cycle
// old_input = last input LAST cycle (inlcuding no input)
// last_char = last input LAST cycle (excluding no input)
char input = '\0', old_input = 'Z', last_char = 'X';

// Do NOT modify shell_buffer_1 or shell_buffer_2
// Use curr_shell_buffer as an interface
char shell_buffer_1[SHELL_BUFFER_SIZE+1] = PROMPT,
	 shell_buffer_2[SHELL_BUFFER_SIZE+1] = PROMPT,
	 *curr_shell_buffer = shell_buffer_1;

// convention: points to the next free position of the buffer (like a stack)
int shell_buffer_pos = MIN_SHELL_BUFFER_POS;

// Helpers to use the buffers
void change_buffer() {
	curr_shell_buffer = curr_shell_buffer == shell_buffer_1 ? shell_buffer_2 : shell_buffer_1;
}

// Modifies curr buffer in place
void modify_curr_buffer(char c) {
	change_buffer();
	curr_shell_buffer[shell_buffer_pos] = c;
}

// Resets both buffers to PROMPT
void reset_buffers() {
	shell_buffer_pos = MIN_SHELL_BUFFER_POS;
	for (int i = 0; i < MIN_SHELL_BUFFER_POS; i++) {
		shell_buffer_1[i] = PROMPT[i];
		shell_buffer_2[i] = PROMPT[i];
	}
	for (int i = MIN_SHELL_BUFFER_POS; i < SHELL_BUFFER_SIZE; i++) {
		shell_buffer_1[i] = '\0';
		shell_buffer_2[i] = '\0';
	}
}

// Effectively decrements the buffer and fills the last position with a null terminator
void decrement_shell_buffer_pos() {
	curr_shell_buffer[shell_buffer_pos] = '\0';
	change_buffer();
	curr_shell_buffer[shell_buffer_pos] = '\0';
	shell_buffer_pos--;
}

// Inserts c after incrementing
void increment_shell_buffer_pos(char c) {
	char curr_tail = curr_shell_buffer[shell_buffer_pos];
	change_buffer();
	curr_shell_buffer[shell_buffer_pos] = curr_tail;
	shell_buffer_pos++;
	curr_shell_buffer[shell_buffer_pos] = c;
}

char letters[10][5] = {
    { '\0', '\0', '\0', '\0', '\0' },	// 0
    { '1' , '-' , '\0', '\0', '\0' },	// 1
    { '2' , 'a' , 'b' , 'c' , '\0' },   // 2
    { '3' , 'd' , 'e' , 'f' , '\0' },   // 3
    { '4' , 'g' , 'h' , 'i' , '\0' },   // 4
    { '5' , 'j' , 'k' , 'l' , '\0' },   // 5
    { '6' , 'm' , 'n' , 'o' , '\0' },   // 6
    { '7' , 'p' , 'q' , 'r' , 's'  },   // 7
    { '8' , 't' , 'u' , 'v' , '\0' },   // 8
    { '9' , 'w' , 'x' , 'y' , 'z'  }    // 9
};



// VIEW MODE FLAGS
enum mode_t {
	SHELL = 0,
	VIEW  = 1
} curr_mode = SHELL;
// Current page being displayed of current command,
// current command index being displayed in cmds[] and cmd_lens[]
int curr_cmd_index, curr_view_line;



void del() {
	if (curr_shell_buffer[shell_buffer_pos] == '\0') decrement_shell_buffer_pos();
	modify_curr_buffer('\0');
}

void submit() {
	// First purpose: to "lock in" a user's choice
	if (curr_shell_buffer[shell_buffer_pos] != '\0') {
		increment_shell_buffer_pos('\0');
		return;
	}

	// Second purpose: to actually submit input
	char input_buffer[SHELL_BUFFER_SIZE+1];
	int input_len = 0;

	// clear BOTH buffers + transfer to input_buffer in 1 go
	for (int i = MIN_SHELL_BUFFER_POS; shell_buffer_pos > MIN_SHELL_BUFFER_POS && curr_shell_buffer[i] != '\0'; i++, shell_buffer_pos--) {
		input_buffer[i-MIN_SHELL_BUFFER_POS] = curr_shell_buffer[i];
		input_len++;
		curr_shell_buffer[i] = '\0'; change_buffer(); curr_shell_buffer[i] = '\0';
	}

	// Match input to a command
	for (int i = 0; i < cmd_count; i++) {
		bool match = curr_mode != VIEW;
		const int cmd_name_len = cmd_name_lens[i];

		if (cmd_name_len != input_len) continue;

		for (int j = 0; j < cmd_name_lens[i] && input_buffer[j] != '\0'; j++)
			match = match && (cmd_names[i][j] == input_buffer[j]);

		if (match) {
			curr_mode = VIEW; curr_cmd_index = i; curr_view_line = 0;
		}
	}
}

void add() {
	// Multi-tap keys => need a dedicated counter
	// Resets when new key pressed
	static int curr_cycle = 0;

	if (input == '#' || input == '*') return;

	curr_cycle = input == last_char ? (curr_cycle + 1) % 4 : 0;

	char char_to_push = letters[(int)input - '0'][curr_cycle];
	if (char_to_push == '\0') return;

	modify_curr_buffer(char_to_push); 
}

void handle_shell_input() {
    if (input != '\0' && input != old_input) {
		if (input == '*' && shell_buffer_pos > MIN_SHELL_BUFFER_POS) del();

		else if (input == '#') submit();

		else {
			// QOL - advance a space if user changes character
			if (curr_shell_buffer[shell_buffer_pos] != '\0' && last_char != input) submit();
			add();
		}

		last_char = input;
	}

	old_input = input;
}



// move curr_view_line by a certain amount safely
void mv_line(const int sign) {
	if ((curr_view_line + sign) * LINE_WIDTH > cmd_lens[curr_cmd_index]
			|| (curr_view_line + sign) < 0) return;
	curr_view_line += sign;
}

void handle_view_input() {
	switch (input) {
		case '\0':
			break;
		case '#':
			if (old_input != input) mv_line(1);
			break;
		case '*':
			if (old_input != input) mv_line(-1);
			break;
		default: // Any other key will return to the shell
			curr_mode = SHELL;
			reset_buffers();
            break;
	}

	old_input = input;
}



void handle_input() { curr_mode == SHELL ? handle_shell_input() : handle_view_input(); }



void draw_screen_internal(const char *buffer, const int len) {
	static const char* old_buffer = NULL; static int old_len = 0;

	// Skip renders that are unecessary
	if (buffer == old_buffer && len == old_len) goto after_render;

	u8g2_ClearBuffer(&u8g2);
	for (int i = 0; i < len; i+= LINE_WIDTH) {
		char line_buffer[LINE_WIDTH+1];
		
		for (int j = 0; j < LINE_WIDTH; j++) {
            line_buffer[j] = buffer[i+j];
        }

		u8g2_DrawStr(&u8g2,
				0,
				TOP_LINE_Y + i / LINE_WIDTH * LINE_HEIGHT,
				line_buffer);

		if (curr_mode == SHELL && i < shell_buffer_pos && i + LINE_WIDTH > shell_buffer_pos) {
			u8g2_DrawStr(&u8g2,
				(shell_buffer_pos - i % LINE_WIDTH) * DISPLAY_CHAR_WIDTH + SPACE_WIDTH,
				TOP_LINE_Y + i / LINE_WIDTH * LINE_HEIGHT + CURSOR_VERT_OFFSET,
				CURSOR
			);
		}
	}
	u8g2_SendBuffer(&u8g2);

after_render: old_buffer = buffer; old_len = len;
}



void draw_screen() {
	curr_mode == SHELL
		? draw_screen_internal(curr_shell_buffer, shell_buffer_pos)
		: draw_screen_internal(cmds[curr_cmd_index] + curr_view_line * LINE_WIDTH,
				cmd_lens[curr_cmd_index] - curr_view_line * LINE_WIDTH);
}
