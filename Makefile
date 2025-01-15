CC=gcc
CFLAGS=-Wall -Wextra
LIBS=-l:raylib.dll

sample: sample.c
	$(CC) $(CFLAGS) -I./ -I./raylib-5.5_win64_mingw-w64/include -o $@ $< -L./ $(LIBS)
