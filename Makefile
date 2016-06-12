# TODO: move bins to a bin dir, add to gitignore
# TODO: library build?

# Only used during development, will be removed
EXE=ujson-dev
_EXESRC=ujson-dev.c

UJ2J=uj2j
_UJ2JSRC=uj2j.c

J2UJ=j2uj
_J2UJSRC=j2uj.c

# Created and run by make tests
TESTS=ujson-tests
_TESTSSRC=ujson-tests.c

# Header file dependencies (TODO not used right now)
_DEPS=ujson-fromjson.h ujson-format.h ujson-tojson.h ujson-dump.h ujson-object.h ujson-array.h ujson-value.h ujson-types.h ujson-string.h ujson-hash.h ujson-encode.h ujson-extract.h ujson-data.h ujson-decode.h hexdump.h ujson-endian.h udp.h schematags.h ujsizes.h ujson-movebytes.h
IDIR=inc
DEPS=$(pasubst %,$(IDIR)/%,$(_DEPS))

# List of source files, excluding the EXE and TESTS runnables above
_SOURCES=ujson-fromjson.c ujson-format.c ujson-tojson.c ujson-dump.c ujson-object.c ujson-array.c ujson-value.c ujson-string.c ujson-hash.c ujson-encode.c ujson-extract.c ujson-data.c ujson-decode.c hexdump.c udp.c ujson-endian.c ujson-movebytes.c
SDIR=src
SOURCES=$(patsubst %,$(SDIR)/%,$(_SOURCES))
EXESRC=$(patsubst %,$(SDIR)/%,$(_EXESRC))
UJ2JSRC=$(patsubst %,$(SDIR)/%,$(_UJ2JSRC))
J2UJSRC=$(patsubst %,$(SDIR)/%,$(_J2UJSRC))
TESTSSRC=$(patsubst %,$(SDIR)/%,$(_TESTSSRC))

_OBJECTS=$(_SOURCES:.c=.o)
ODIR=obj
$(shell mkdir -p $(ODIR) >/dev/null)
OBJECTS=$(patsubst %,$(ODIR)/%,$(_OBJECTS))
EXEOBJ=$(patsubst %,$(ODIR)/%,$(_EXESRC:.c=.o))
UJ2JOBJ=$(patsubst %,$(ODIR)/%,$(_UJ2JSRC:.c=.o))
J2UJOBJ=$(patsubst %,$(ODIR)/%,$(_J2UJSRC:.c=.o))
TESTSOBJ=$(patsubst %,$(ODIR)/%,$(_TESTSSRC:.c=.o))

_D=$(_OBJECTS:.o=.d) $(EXE).d $(TESTS).d
DDIR=d
$(shell mkdir -p $(DDIR) >/dev/null)
D=$(patsubst %,$(DDIR)/%,$(_D))

CC=gcc
CFLAGS=-g -Wall -I$(IDIR) -std=c99
LDFLAGS=
DEPFLAGS=-MT $@ -MMD -MP -MF $(DDIR)/$*.Td
POSTCOMPILE=mv -f $(DDIR)/$*.Td $(DDIR)/$*.d

# TODO During dev this builds and runs the dev exe for the default make cmd
all: $(SOURCES) $(EXESRC) $(EXE) runit

runit:
	./$(EXE)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@
	$(POSTCOMPILE)

$(UJ2J): $(OBJECTS) $(UJ2JOBJ)
	$(CC) $(LDFLAGS) $(OBJECTS) $(UJ2JOBJ) -o $@

$(J2UJ): $(OBJECTS) $(J2UJOBJ)
	$(CC) $(LDFLAGS) $(OBJECTS) $(J2UJOBJ) -o $@

$(EXE): $(OBJECTS) $(EXEOBJ)
	$(CC) $(LDFLAGS) $(OBJECTS) $(EXEOBJ) -o $@

tests: $(OBJECTS) $(TESTSOBJ)
	$(CC) $(LDFLAGS) $(OBJECTS) $(TESTSOBJ) -o $(TESTS)
	./$(TESTS)

.PHONY clean:
	rm -f $(OBJECTS) $(EXE) $(EXEOBJ) $(UJ2J) $(UJ2JOBJ) $(J2UJ) $(J2UJOBJ) $(TESTSOBJ) $(TESTS) $(D)

-include $(D)
