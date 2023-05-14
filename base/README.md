# C1B3RB4DG3 - Base library

Basic library, useful to implement programs for the c1b3rb4dg3.

## Contents

The base library loads the libraries that will be used in the project. Such as
ezButton and Arduino GFX Library. It also defines some macros, including
the pins used by the badge or the screen size.

The library defines the variables representing the buttons, the screen
and the leds. It also defines some utility functions (e.g. `setup_badge()`).

## How to use

To use the library, you must first include it in your program. To do so,
add the following line at the beginning of your program:

```c++
#include 'base.h'
```

Then, you must call the `setup_badge()` function in the `setup()` function
of your sketch. This function initializes the badge and must be called
before any other function of the library.

```c++
void setup() {
    setup_badge();
    // ...
}
```

Then, you can use the variables and functions defined in the library. To see
an example of how to use the library, check `TODO: make simple example`.

Finally, to upload the sketch to the board you can use the following command:

```bash
# assuming the following directory structure:
# 
# c1b3rb4dg3
# ├── base
# │   ├── base.cpp
# │   └── base.h
# └── your_project
#     └── your_project.ino

cd your_project
arduino-cli compile -b esp32:esp32:esp32 -u -p <PORT> --library ../base
```

You should replace `<PORT>` with the port where the badge is connected (e.g.
`/dev/ttyUSB0`).