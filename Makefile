SUBDIRS = $(wildcard ex*)

.PHONY: clean subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

bin:
	mkdir bin

$(SUBDIRS): bin
	$(MAKE) -C $@

clean:
	@for dir in $(SUBDIRS); do $(MAKE) $@ -C $$dir; done
