include ../env.mk
all: libcutils.dylib libutils.dylib
STUFF := -DHAVE_MACOSX_IPC -DOS_PATH_SEPARATOR="'/'" -DHAVE_PTHREADS -DHAVE_POSIX_FILEMAP -DHAVE_LITTLE_ENDIAN -I. -Ilibcutils 
libcutils.dylib: 
	$(GCC) $(STUFF) -dynamiclib -o libcutils.dylib libcutils/*.c
libutils.dylib:
	$(GXX) $(STUFF) -dynamiclib -o libutils.dylib utils/* -L. -lcutils -lz
clean:
	rm -f libutils.dylib libcutils.dylib
