#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeclips.a

CPPFLAGS += -I../../../include/openeaagles/ubf/clips/clips

OBJS += \
	clips/agenda.o \
	clips/analysis.o \
	clips/argacces.o \
	clips/bload.o \
	clips/bmathfun.o \
	clips/bsave.o \
	clips/classcom.o \
	clips/classexm.o \
	clips/classfun.o \
	clips/classinf.o \
	clips/classini.o \
	clips/classpsr.o \
	clips/clipscpp.o \
	clips/clsltpsr.o \
	clips/commline.o \
	clips/conscomp.o \
	clips/constrct.o \
	clips/constrnt.o \
	clips/crstrtgy.o \
	clips/cstrcbin.o \
	clips/cstrccom.o \
	clips/cstrcpsr.o \
	clips/cstrnbin.o \
	clips/cstrnchk.o \
	clips/cstrncmp.o \
	clips/cstrnops.o \
	clips/cstrnpsr.o \
	clips/cstrnutl.o \
	clips/default.o \
	clips/defins.o \
	clips/developr.o \
	clips/dffctbin.o \
	clips/dffctbsc.o \
	clips/dffctcmp.o \
	clips/dffctdef.o \
	clips/dffctpsr.o \
	clips/dffnxbin.o \
	clips/dffnxcmp.o \
	clips/dffnxexe.o \
	clips/dffnxfun.o \
	clips/dffnxpsr.o \
	clips/dfinsbin.o \
	clips/dfinscmp.o \
	clips/drive.o \
	clips/edbasic.o \
	clips/edmain.o \
	clips/edmisc.o \
	clips/edstruct.o \
	clips/edterm.o \
	clips/emathfun.o \
	clips/engine.o \
	clips/envrnmnt.o \
	clips/evaluatn.o \
	clips/expressn.o \
	clips/exprnbin.o \
	clips/exprnops.o \
	clips/exprnpsr.o \
	clips/extnfunc.o \
	clips/factbin.o \
	clips/factbld.o \
	clips/factcmp.o \
	clips/factcom.o \
	clips/factfun.o \
	clips/factgen.o \
	clips/facthsh.o \
	clips/factlhs.o \
	clips/factmch.o \
	clips/factmngr.o \
	clips/factprt.o \
	clips/factqpsr.o \
	clips/factqury.o \
	clips/factrete.o \
	clips/factrhs.o \
	clips/filecom.o \
	clips/filertr.o \
	clips/generate.o \
	clips/genrcbin.o \
	clips/genrccmp.o \
	clips/genrccom.o \
	clips/genrcexe.o \
	clips/genrcfun.o \
	clips/genrcpsr.o \
	clips/globlbin.o \
	clips/globlbsc.o \
	clips/globlcmp.o \
	clips/globlcom.o \
	clips/globldef.o \
	clips/globlpsr.o \
	clips/immthpsr.o \
	clips/incrrset.o \
	clips/inherpsr.o \
	clips/inscom.o \
	clips/insfile.o \
	clips/insfun.o \
	clips/insmngr.o \
	clips/insmoddp.o \
	clips/insmult.o \
	clips/inspsr.o \
	clips/insquery.o \
	clips/insqypsr.o \
	clips/iofun.o \
	clips/lgcldpnd.o \
	clips/memalloc.o \
	clips/miscfun.o \
	clips/modulbin.o \
	clips/modulbsc.o \
	clips/modulcmp.o \
	clips/moduldef.o \
	clips/modulpsr.o \
	clips/modulutl.o \
	clips/msgcom.o \
	clips/msgfun.o \
	clips/msgpass.o \
	clips/msgpsr.o \
	clips/multifld.o \
	clips/multifun.o \
	clips/objbin.o \
	clips/objcmp.o \
	clips/objrtbin.o \
	clips/objrtbld.o \
	clips/objrtcmp.o \
	clips/objrtfnx.o \
	clips/objrtgen.o \
	clips/objrtmch.o \
	clips/parsefun.o \
	clips/pattern.o \
	clips/pprint.o \
	clips/prccode.o \
	clips/prcdrfun.o \
	clips/prcdrpsr.o \
	clips/prdctfun.o \
	clips/prntutil.o \
	clips/proflfun.o \
	clips/reorder.o \
	clips/reteutil.o \
	clips/retract.o \
	clips/router.o \
	clips/rulebin.o \
	clips/rulebld.o \
	clips/rulebsc.o \
	clips/rulecmp.o \
	clips/rulecom.o \
	clips/rulecstr.o \
	clips/ruledef.o \
	clips/ruledlt.o \
	clips/rulelhs.o \
	clips/rulepsr.o \
	clips/scanner.o \
	clips/sortfun.o \
	clips/strngfun.o \
	clips/strngrtr.o \
	clips/symblbin.o \
	clips/symblcmp.o \
	clips/symbol.o \
	clips/sysdep.o \
	clips/textpro.o \
	clips/tmpltbin.o \
	clips/tmpltbsc.o \
	clips/tmpltcmp.o \
	clips/tmpltdef.o \
	clips/tmpltfun.o \
	clips/tmpltlhs.o \
	clips/tmpltpsr.o \
	clips/tmpltrhs.o \
	clips/tmpltutl.o \
	clips/userdata.o \
	clips/userfunctions.o \
	clips/utility.o \
	clips/watch.o \
	factory.o \
	ClipsArbiter.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f clips/*.o
	-rm -f $(LIB)
