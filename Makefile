
# Libraries
LIBS = src

all:
	for subdir in $(LIBS); do \
	  echo making $@ in $$subdir; \
	  (cd $$subdir && $(MAKE)) || exit 1; \
	done

clean:
	for subdir in $(LIBS); do \
	    echo $@ in $$subdir; \
	    (cd $$subdir && $(MAKE) $@) || exit 1; \
	done

