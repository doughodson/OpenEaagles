#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeBasicGL.a

OBJS =  \
	$(LIB)(basicGLFF.o) \
	$(LIB)(BmpTexture.o) \
	$(LIB)(BitmapFont.o) \
	$(LIB)(Clip3D.o) \
	$(LIB)(ColorGradient.o) \
	$(LIB)(ColorRotary.o) \
	$(LIB)(Display.o) \
	$(LIB)(Factory.o) \
	$(LIB)(Field.o) \
	$(LIB)(Font.o) \
	$(LIB)(FtglFonts.o) \
	$(LIB)(Graphic.o) \
	$(LIB)(Image.o) \
	$(LIB)(Material.o) \
	$(LIB)(Page.o) \
	$(LIB)(Polygon.o) \
	$(LIB)(Readouts.o) \
	$(LIB)(Reformat.o) \
	$(LIB)(Rotators.o) \
	$(LIB)(Scanline.o) \
	$(LIB)(Shapes.o) \
	$(LIB)(StrokeFont.o) \
	$(LIB)(Texture.o) \
	$(LIB)(MfdPage.o) \
	$(LIB)(MapPage.o) \
	$(LIB)(SymbolLoader.o) \
	$(LIB)(Translator.o)

CPPFLAGS += -I$(FREETYPE2_INC_DIR)

all: ${OBJS}

#Reformat.cpp: Reformat.l
#	dos2unix Reformat.l
#	flex -f -oReformat.cpp Reformat.l

clean:
	-rm -f *.o
	-rm -f $(LIB)

