all: libcutils.dylib libutils.dylib
STUFF := -DHAVE_MACOSX_IPC -DOS_PATH_SEPARATOR="'/'" -DHAVE_PTHREADS -DHAVE_POSIX_FILEMAP -DHAVE_LITTLE_ENDIAN -I. -Ilibcutils -isysroot /var/sdk -arch armv7
libcutils.dylib: 
	gcc-4.2 $(STUFF) -dynamiclib -o libcutils.dylib libcutils/*.c
libutils.dylib:
	g++-4.2 $(STUFF) -dynamiclib -o libutils.dylib utils/* -L. -lcutils -lz
clean:
	rm -f libutils.dylib libcutils.dylib
