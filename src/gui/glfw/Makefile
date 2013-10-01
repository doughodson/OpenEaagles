#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeGlfw.a

OBJS =  \
	$(LIB)(glfwFF.o) \
	$(LIB)(GlfwDisplay.o) 

all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

