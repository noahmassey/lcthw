CFLAGS=-Wall -g

OUTDIR=../bin
TARGET=$(OUTDIR)/ex13 $(OUTDIR)/ex13a

CFILES = $(wildcard *.c)
OFILES = $(patsubst %c,%o,$(CFILES))

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

$(OUTDIR)/%: %.o
	cc $(CFLAGS) -o $@ $^

all: $(TARGET)


clean:
	rm -f $(OFILES) $(TARGET)
