CC=gcc
CFLAGS=-Wall -Wextra

sample: sample.c
	$(CC) $(CFLAGS) -I./ -o $@ $<
