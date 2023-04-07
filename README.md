# TodoMaster

A program to generate an organized file with all your **TODO** and **FIXME** tasks.

## Usage

Example main.c:

```c
#include <stdio.h>

// TODO: Make test do something.
int test() {}

// FIXME: Fix main.
// TODO: Add some functionality to the program.
int main() {
    printf("Hello World!%s\n", test(); // FIXME: change %s to %d.

    return 0;
}
```

Execute the program: `./todomaster main.c`

Generated main.c.TodoMaster:

```
------TODO-----
1. TODO (L3 C27) : Make test do something.
2. TODO (L7 C4) : Add some functionality to the program.

-----FIXME-----
1. FIXME (L6 C19) : Fix main.
2. FIXME (L9 C56) : change %s to %d.
```

Usage: `./todomaster <input-file> [-o/--ouput <output-file>]/[-h/--help]`

## Installation

Install from source.

```bash
$ git clone https://github.com/Iamlel/TodoMaster.git
$ cd TodoMaster
$ gcc TodoMaster -o todomaster
```

## License

Distributed under the GNU General Public License v3.0. See `LICENSE.txt` for more information.

## Acknowledgments

[Linked list library that was modified for this project.](https://github.com/skorks/c-linked-list/tree/master)
