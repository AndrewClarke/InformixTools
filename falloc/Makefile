
INSTDIR = /usr/local/bin

FALLOC = falloc
FTRUNC = ftrunc

CFLAGS = -Wall -Wextra

all: $(INSTDIR)/$(FALLOC) $(INSTDIR)/$(FTRUNC)

clean:
	-rm -f *.o $(FALLOC) $(FTRUNC)

$(INSTDIR)/$(FALLOC): $(FALLOC)
	strip $(FALLOC)
	cp $(FALLOC) $(INSTDIR)/$(FALLOC)

$(INSTDIR)/$(FTRUNC): $(FTRUNC)
	strip $(FTRUNC)
	cp $(FTRUNC) $(INSTDIR)/$(FTRUNC)

$(FALLOC): lspec.o $(FALLOC).o

$(FTRUNC): lspec.o $(FTRUNC).o

