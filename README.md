# Explore

A video game where you explore the surroundings.

To compile ncurses menu use:
- gcc -I/opt/homebrew/opt/ncurses/include -L/opt/homebrew/opt/ncurses/lib src/_menu.c -o menu -lmenu -lncurses

To compile sdl2:
- gcc src/_sdl.c -o game -I /opt/homebrew/opt/sdl2/include -L /opt/homebrew/opt/sdl2/lib -lSDL2


To build from source:
- cd into project
- ./configure
- make
- make install
