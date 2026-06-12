CC ?= cc
CFLAGS ?= -std=c99 -Wall -Wextra -pedantic
SDL_CFLAGS := $(shell pkg-config --cflags sdl3)
SDL_LIBS := $(shell pkg-config --libs sdl3)

.PHONY: all clean run

all: compute_sum

sum.comp.spv: sum.comp
	glslangValidator -V -S comp $< -o $@

compute_sum: main.c sum.comp.spv
	$(CC) $(CFLAGS) $(SDL_CFLAGS) main.c -o $@ $(SDL_LIBS)

run: compute_sum
	./compute_sum

clean:
	rm -f compute_sum sum.comp.spv
