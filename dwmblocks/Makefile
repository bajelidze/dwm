PREFIX ?= ${HOME}/.local
CC ?= cc
LDFLAGS = -lX11

output: dwmblocks.c blocks.def.h blocks.h
	${CC}  dwmblocks.c $(LDFLAGS) -o dwmblocks
blocks.h:
	cp blocks.def.h $@

clean:
	rm -f *.o *.gch dwmblocks
install: output
	mkdir -p $(PREFIX)/bin
	install -m 0755 dwmblocks $(PREFIX)/bin/dwmblocks
uninstall:
	rm -f $(PREFIX)/bin/dwmblocks
