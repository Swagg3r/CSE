CC=gcc

# uncomment to compile in 32bits mode (require gcc-*-multilib packages
# on Debian/Ubuntu)
#HOST32= -m32

CPPFLAGS= -I.
CFLAGS= $(HOST32) -Wall -Werror -std=c99 -g -fPIC -DMEMORY_SIZE=128000
LDFLAGS= $(HOST32)


.PHONY: clean dist

all: stdes
	

# dépendences des binaires
memshell: stdes.o

# nettoyage
clean:
	rm -f *.o stdes .*.deps

# dépendances des fichiers objects
$(patsubst %.c,%.o,$(wildcard *.c)): %.o: .%.deps

.%.deps: %.c
	$(CC) $(CPPFLAGS) -MM $< | sed -e 's/\(.*\).o: /.\1.deps \1.o: /' > $@

-include $(wildcard .*.deps)