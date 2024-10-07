all:
	cc src/*.c -Wall -lSDL2 -lSDL2_image -O3

debug:
	cc src/*.c -Wall -lSDL2 -lSDL2_image -O3 -ggdb -fsanitize=address,undefined
