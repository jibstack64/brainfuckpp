# brainfuck++

![GitHub Release](https://img.shields.io/github/v/release/jibstack64/brainfuckpp)

An extension of the `brainfuck` programming language.

## Usage

Releases of both the compiler and interpreter can be found on the Releases page.

### Compiler

The `brainfuck++` compiler essentially packages the interpreter with your code so that you do not have to distribute the two separately.

It is written in Python and such you must have Python (3.10>) installed to use it.

`python compiler.py "+++>>+...`

### Interpreter

The interpreter parses `brainfuck`/`brainfuck++` code and runs it.

`./brainfuck++ "+++>>+...`

## Logic

### Changes

In regular `brainfuck`, the size of the array that you operate within is 30kb, where each cell is a single byte. In `brainfuck++`, the array has been made 2-dimensional - each cell is 8 bytes to allow for a wider range of numbers and the "width" and "height" of the array is 255 cells.

### Operators

#### Comments
Any text wrapped in `#`s is ignored by the interpreter.

#### Movement
- `>` move the x pointer right.
- `<` move the x pointer left.
- `^` move the y pointer up.
- `v` move the y pointer down.

#### Loops
- `[...]` runs the code within the square brackets until the value of the current cell is 0. In C this would look like:
    ```c
    while (*current_cell != 0)
        run(code);
    ```
- `(...)` essentially the same as `[]`, but instead of waiting for the current cell to be 0, it binds to the cell the loop is constructed at and decrements one from it every loop. Again, a simple C representation:
    ```c
    long *bound = current_cell;
    while (*bound != 0) {
        run(code);
        (*bound)--;
    }
    ```
If the value of the bound cell is under 0, then its value will be *incremented* until 0 is reached.

#### I/O
- `.` prints the value of the current cell.
- `~` prints the value of every cell on the current x line.
- `|` prints the value of every cell across the current y line.
- `,` gets a character from the user and stores it in the current cell.

#### Memory
A separate cell is allocated for storing temporary memory. `$` sets the value of the memory to that of the current cell. Using `$` when the value stored within the memory is not 0 sets the value of the current cell to that of the memory and resets the memory to 0.

#### Functions
`{...}` defines a function. There are 10 function slots - each function can be called by its identifier (`0-9`).
Once 10 functions have been defined, trying to create a new one generates errors. A basic example:
```bf
{++++++++++} # Function '0', adds 10 to current cell. #
0
. # Should print a '\n' as '\n' = 0. #
```

#### Maths
- `+` increments the value of the current cell by 1.
- `-` decrements the value of the current cell by 1.
- `*` doubles the value of the current cell.
- `/` halves the value of the current cell.

