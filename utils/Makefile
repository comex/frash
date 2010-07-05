all: libcutils.dylib libutils.dylib
STUFF := -DHAVE_MACOSX_IPC -DOS_PATH_SEPARATOR="'/'" -DHAVE_PTHREADS -DHAVE_POSIX_FILEMAP -DHAVE_LITTLE_ENDIAN -I. -Ilibcutils -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS3.2.sdk/ -arch armv7
libcutils.dylib: 
	gcc-4.2 $(STUFF) -dynamiclib -o libcutils.dylib libcutils/*.c
libutils.dylib:
	g++-4.2 $(STUFF) -dynamiclib -o libutils.dylib utils/* -L. -lcutils -lz
clean:
	rm -f libutils.dylib libcutils.dylib
