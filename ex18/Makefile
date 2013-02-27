EXAMPLE = ex18

OUTDIR = ../bin/
TARGET = $(OUTDIR)$(EXAMPLE)

CFLAGS=-Wall -g

CFILES = $(wildcard *.c)
OFILES = $(patsubst %c,%o,$(CFILES))

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

$(TARGET): $(OFILES)
	cc $(CFLAGS) -o $@ $^

clean:
	rm -f $(OFILES) $(TARGET)
