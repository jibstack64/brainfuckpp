
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>

/* Keys */
#define POINTER_UP      '^'
#define POINTER_DOWN    'v'
#define POINTER_LEFT    '<'
#define POINTER_RIGHT   '>'
#define CELL_INCREMENT  '+'
#define CELL_DECREMENT  '-'
#define CELL_DOUBLE     '*'
#define CELL_HALF       '/'
#define LOOP_OPEN       '['
#define LOOP_CLOSE      ']'
#define BLOOP_OPEN      '('
#define BLOOP_CLOSE     ')'
#define FUNCTION_OPEN   '{'
#define FUNCTION_CLOSE  '}'
#define FUNCTION_0      '0'
#define FUNCTION_1      '1'
#define FUNCTION_2      '2'
#define FUNCTION_3      '3'
#define FUNCTION_4      '4'
#define FUNCTION_5      '5'
#define FUNCTION_6      '6'
#define FUNCTION_7      '7'
#define FUNCTION_8      '8'
#define FUNCTION_9      '9'
#define PRINT_CELL      '.'
#define PRINT_X         '~'
#define PRINT_Y         '|'
#define INPUT_CELL      ','
#define MEMORY_TOGGLE   '$'
#define COMMENT         '#'

/* All initialised within `init`. */
long cells[UINT8_MAX][UINT8_MAX],
     memory;
char *functions[10];
uint8_t x, y;

void init() {
    memset(cells, 0, 255 * 255 * sizeof(long));
    memset(functions, 0, 10 * sizeof(char *));
    memory = 0;
    x = 0, y = 0;
}

void cleanup() {
    for (uint8_t i = 0; i < 10; i++)
        if (functions[i] != 0)
            free(functions[i]);
}

/* Exits with `text`. */
void error(const char *text) {
    cleanup();
    printf("%s.\n", text);
    exit(1);
}

/* Iterates through `code` until `code[i] == until`. */
size_t capture(const char *code, char start, char until) {
    if (start != 0) {
        int depth = 0;
        for (size_t p = 0; code[p]; p++) {
            if (code[p] == start) {
                depth++;
            }
            if (code[p] == until) {
                if (depth < 0) return -1;
                else if (depth == 0) return p;
                depth--;
            }
        }
    } else {
        for (size_t p = 0; code[p]; p++) {
            if (code[p] == until)
                return p;
        }
    }
    return -1;
}

/* Iterates through each character of `code` and carries out the relevant
 * operation. */
void run(const char *code) {

    for (const char *current = code; *current; current++) {  
        
        char value = *current;

        switch (value) {

            case POINTER_UP:
                y--;
                break;
            case POINTER_DOWN:
                y++;
                break;
            case POINTER_LEFT:
                x--;
                break;
            case POINTER_RIGHT:
                x++;
                break;

            case CELL_INCREMENT:
                cells[x][y]++;
                break;
            case CELL_DECREMENT:
                cells[x][y]--;
                break;
            case CELL_DOUBLE:
                cells[x][y] *= 2;
                break;
            case CELL_HALF:
                cells[x][y] /= 2;
                break;

            case LOOP_OPEN: {
                size_t until = capture(current + 1, LOOP_OPEN, LOOP_CLOSE);
                if (until == -1) {
                    error("Loop not closed");
                }
                char buffer[until + 1];
                strncpy(buffer, current + 1, until);
                buffer[until] = '\0';
                while (cells[x][y] != 0)
                    run(buffer);
                current += until + 1;
                break;
            }
            case LOOP_CLOSE:
                error("Loop close outside of loop");
                break;

            case BLOOP_OPEN: {
                size_t until = capture(current + 1, BLOOP_OPEN, BLOOP_CLOSE);
                if (until == -1) {
                    error("Bound-loop not closed");
                }
                char buffer[until + 1];
                strncpy(buffer, current + 1, until);
                buffer[until] = '\0';
                long *cell = &(cells[x][y]);
                for (; *cell != 0; (*cell) += (*cell < 0 ? 1 : -1))
                    run(buffer);
                current += until + 1;
                break;
            }
            case BLOOP_CLOSE:
                error("Bound-loop close outside of loop");
                break;

            case FUNCTION_OPEN: {
                size_t until = capture(current + 1, FUNCTION_OPEN, FUNCTION_CLOSE);
                if (until == -1) {
                    error("Function not closed");
                }
                // Find an open function
                int i = 0;
                for (; functions[i] != 0 && i < 12; i++);
                if (i == 11) {
                    error("All function slots taken");
                }
                // Store functions
                char *buffer = (functions[i] = malloc(
                            sizeof(char) * (until + 1)));
                strncpy(buffer, current + 1, until);
                buffer[until] = '\0';
                current += until + 1;
                break;
            }
            case FUNCTION_CLOSE:
                error("Function close outside of function");
                break;
            case FUNCTION_0:
            case FUNCTION_1:
            case FUNCTION_2:
            case FUNCTION_3:
            case FUNCTION_4:
            case FUNCTION_5:
            case FUNCTION_6:
            case FUNCTION_7:
            case FUNCTION_8:
            case FUNCTION_9: {
                uint8_t function = value - 48; // 48 = '0' in ASCII 
                if (functions[function] == 0) {
                    error("Function not defined");
                }
                run(functions[function]);
                break;
            }

            case PRINT_CELL:
                /*  This may result in loss of information...
                    Why would you be printing a value outside of the ASCII
                    table anyways?  */
                putchar((char)cells[x][y]);
                break;
            case PRINT_X:
                for (uint8_t p = 0; p < UINT8_MAX; p++)
                    putchar((char)cells[p][y]);
                break;
            case PRINT_Y:
                for (uint8_t p = 0; p < UINT8_MAX; p++)
                    putchar((char)cells[x][p]);
                break;

            case INPUT_CELL:
                cells[x][y] = getchar();
                break;

            case MEMORY_TOGGLE:
                if (memory == 0)
                    memory = cells[x][y];
                else {
                    cells[x][y] = memory;
                    memory = 0;
                }
                break;

            case COMMENT: {
                size_t until = capture(current + 1, 0, COMMENT);
                if (until == -1) {
                    exit(0);
                }
                current += until + 1;
                break;
            }
            
            case ' ':
            case '\t':
            case '\n':
                break;

            default:
                error("Unknown symbol");
                break;

        }
    }
}
