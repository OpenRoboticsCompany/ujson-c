# TODO: move bins to a bin dir, add to gitignore
# TODO: library build?

# Only used during development, will be removed
EXE=ujson-dev
_EXESRC=ujson-dev.c

# Created and run by make tests
TESTS=ujson-tests
_TESTSSRC=ujson-tests.c

# Header file dependencies (TODO not used right now)
_DEPS=ujson-tojson.h ujson-dump.h ujson-object.h ujson-array.h ujson-value.h ujson-types.h ujson-string.h ujson-hash.h ujson-encode.h ujson-extract.h ujson-data.h ujson-parse.h hexdump.h endian.h udp.h schematags.h ujsizes.h movebytes.h
IDIR=inc
DEPS=$(pasubst %,$(IDIR)/%,$(_DEPS))

# List of source files, excluding the EXE and TESTS runnables above
_SOURCES=ujson-tojson.c ujson-dump.c ujson-object.c ujson-array.c ujson-value.c ujson-string.c ujson-hash.c ujson-encode.c ujson-extract.c ujson-data.c ujson-parse.c hexdump.c udp.c endian.c movebytes.c
SDIR=src
SOURCES=$(patsubst %,$(SDIR)/%,$(_SOURCES))
EXESRC=$(patsubst %,$(SDIR)/%,$(_EXESRC))
TESTSSRC=$(patsubst %,$(SDIR)/%,$(_TESTSSRC))

_OBJECTS=$(_SOURCES:.c=.o)
ODIR=obj
$(shell mkdir -p $(ODIR) >/dev/null)
OBJECTS=$(patsubst %,$(ODIR)/%,$(_OBJECTS))
EXEOBJ=$(patsubst %,$(ODIR)/%,$(_EXESRC:.c=.o))
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

$(EXE): $(OBJECTS) $(EXEOBJ)
	$(CC) $(LDFLAGS) $(OBJECTS) $(EXEOBJ) -o $@

tests: $(OBJECTS) $(TESTSOBJ)
	$(CC) $(LDFLAGS) $(OBJECTS) $(TESTSOBJ) -o $(TESTS)
	./$(TESTS)

.PHONY clean:
	rm -f $(OBJECTS) $(EXE) $(EXEOBJ) $(TESTSOBJ) $(TESTS) $(D)

-include $(D)
