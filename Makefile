EXE=ujson-dev
_EXESRC=ujson-dev.c

TESTS=ujson-tests
_TESTSSRC=ujson-tests.c

_DEPS=ujson-render.h hexdump.h endian.h udp.h
IDIR=inc
DEPS=$(pasubst %,$(IDIR)/%,$(_DEPS))

_SOURCES=ujson-render.c hexdump.c udp.c endian.c
SDIR=src
SOURCES=$(patsubst %,$(SDIR)/%,$(_SOURCES))
EXESRC=$(patsubst %,$(SDIR)/%,$(_EXESRC))
TESTSSRC=$(patsubst %,$(SDIR)/%,$(_TESTSSRC))

_OBJECTS=$(_SOURCES:.c=.o)
ODIR=obj
OBJECTS=$(patsubst %,$(ODIR)/%,$(_OBJECTS))
EXEOBJ=$(patsubst %,$(ODIR)/%,$(_EXESRC:.c=.o))
TESTSOBJ=$(patsubst %,$(ODIR)/%,$(_TESTSSRC:.c=.o))

CC=gcc
CFLAGS=-g -Wall -I$(IDIR) -std=c99 -c
LDFLAGS=

all: $(SOURCES) $(EXESRC) $(EXE) runit

runit:
	./$(EXE)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) $< -o $@

$(EXE): $(OBJECTS) $(EXEOBJ)
	$(CC) $(LDFLAGS) $(OBJECTS) $(EXEOBJ) -o $@

tests: $(OBJECTS) $(TESTSOBJ)
	$(CC) $(LDFLAGS) $(OBJECTS) $(TESTSOBJ) -o $(TESTS)
	./$(TESTS)

.PHONY clean:
	rm -f $(OBJECTS) $(EXE) $(EXEOBJ) $(TESTSOBJ) $(TESTS)
