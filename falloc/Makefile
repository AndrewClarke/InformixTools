
FALLOC = /usr/local/bin/falloc
TRUNC = /usr/local/bin/truncate

CFLAGS = -Wall -Wextra

all: $(FALLOC) $(TRUNC)

clean:
	-rm -f *.o falloc truncate

$(FALLOC): falloc
	strip falloc
	cp falloc $(FALLOC)

$(TRUNC): truncate
	strip truncate
	cp truncate $(TRUNC)

falloc: falloc.o lspec.o

truncate: truncate.o lspec.o

