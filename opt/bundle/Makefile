SUBDIRS=src tests
all: $(SUBDIRS)

.PHONY: clean $(SUBDIRS)

$(SUBDIRS):
	for d in $(SUBDIRS); do $(MAKE) -C $$d; done

clean:
	rm -f $(OBJS) *.o
	for d in $(SUBDIRS); do $(MAKE) -C $$d $@; done
