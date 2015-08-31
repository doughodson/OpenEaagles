#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeBasicGL.a

OBJS =  \
	basicGLFF.o \
	BmpTexture.o \
	BitmapFont.o \
	Clip3D.o \
	ColorGradient.o \
	ColorRotary.o \
	Display.o \
	Factory.o \
	Field.o \
	Font.o \
	FtglFonts.o \
	Graphic.o \
	Image.o \
	Material.o \
	Page.o \
	Polygon.o \
	Readouts.o \
	Reformat.o \
	Rotators.o \
	Scanline.o \
	Shapes.o \
	StrokeFont.o \
	Texture.o \
	MfdPage.o \
	MapPage.o \
	SymbolLoader.o \
	Translator.o

CPPFLAGS += -I$(FREETYPE2_INC_DIR)

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

