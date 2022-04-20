PREFIX ?= /usr/local
MANPREFIX ?= $(PREFIX)/share/man
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
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	install -m 0644 someblocks.1 $(DESTDIR)$(MANPREFIX)/man1/someblocks.1
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/someblocks
	rm -f $(DESTDIR)$(MANPREFIX)/man1/someblocks.1
