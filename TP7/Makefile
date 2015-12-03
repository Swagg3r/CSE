CC=gcc

# uncomment to compile in 32bits mode (require gcc-*-multilib packages
# on Debian/Ubuntu)
#HOST32= -m32

CPPFLAGS= -I.
CFLAGS= $(HOST32) -Wall -Werror -std=c99 -g -fPIC -DMEMORY_SIZE=128000000
LDFLAGS= $(HOST32)


.PHONY: clean dist

all: memshell memtest memcustomtest
	./memtest
	./memcustomtest
	

# dépendences des binaires
memshell: mem.o
memtest: mem.o
memcustomtest: mem.o

# nettoyage
clean:
	rm -f *.o memshell memtest memcustomtest libmalloc.so leak_test .*.deps

# dépendances des fichiers objects
$(patsubst %.c,%.o,$(wildcard *.c)): %.o: .%.deps

.%.deps: %.c
	$(CC) $(CPPFLAGS) -MM $< | sed -e 's/\(.*\).o: /.\1.deps \1.o: /' > $@

-include $(wildcard .*.deps)

# seconde partie du sujet
part2: leak_test libmalloc.so
	./leak_test
	$(MAKE) test_ls

libmalloc.so: malloc_stub.o mem.o
	$(CC) -shared -Wl,-soname,$@ $^ -o $@

test_ls: libmalloc.so
	LD_PRELOAD=./libmalloc.so ls

test_gcc: libmalloc.so
	LD_PRELOAD=./libmalloc.so gcc -o hello hello.c

test_make: libmalloc.so
	LD_PRELOAD=./libmalloc.so make

leak_test: malloc_stub.o mem.o

