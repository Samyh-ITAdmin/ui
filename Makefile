CC=gcc
CFLAGS=-Wall -Wextra -I./include -I./raylib-5.0_win64_mingw-w64/include/ -Wswitch-enum -Werror=switch-enum -Wno-char-subscripts -Wno-sign-compare
LDFLAGS=-L./raylib-5.0_win64_mingw-w64/lib
LIBS=-lraylib -lgdi32 -lwinmm

sample: sample.c
	$(CC) $(CFLAGS) -I./ -I./raylib-5.5_linux_amd64/include -o $@ $< -L./ $(LIBS)
