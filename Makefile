PREFIX ?= /usr/local
CC ?= cc

output: someblocks.c blocks.def.h blocks.h
	${CC}  someblocks.c $(LDFLAGS) -o someblocks
blocks.h:
	cp blocks.def.h $@


clean:
	rm -f *.o *.gch someblocks
install: output
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 0755 someblocks $(DESTDIR)$(PREFIX)/bin/someblocks
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/someblocks
