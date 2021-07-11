CC=clang
CFLAGS=-Wall -Werror -Wno-unused -Ivendor -g -fsanitize=address -fsanitize-undefined-trap-on-error #-DDEBUG_H

main: vendor/ztable.o vendor/zwalker.o zjson.o
	$(CC) $(CFLAGS) vendor/ztable.o vendor/zwalker.o zjson.o zjson-test.c -o zjson-test

clean:
	-find -type f -name "*.o" | xargs rm
	-rm zjson
