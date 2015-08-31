
# Libraries
SUBDIRS = src

all: $(SUBDIRS)

clean: $(SUBDIRS)
    
$(SUBDIRS)::
	$(MAKE) -C $@ $(MAKECMDGOALS)

