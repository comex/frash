export SDK := /var/sdk

all:
	make food
STUFF := -mthumb -g3 -Wreturn-type -Wall -DHAVE_MACOSX_IPC -DOS_PATH_SEPARATOR="'/'" -DHAVE_PTHREADS -DHAVE_POSIX_FILEMAP -DHAVE_LITTLE_ENDIAN -I. -I../rpc -I../utils -I../utils/libcutils -isysroot $(SDK) -arch armv7 -ffixed-r9 -Werror 

%.o: %.cpp common.h
	g++-4.2 $(STUFF) -c -o $@ $<

%.o: %.c common.h
	gcc-4.2 $(STUFF) -c -o $@ $<

multistuff.c: multistuff.py
	python multistuff.py

multistuff.settings.h: multistuff.py
	python multistuff.py

myjni.o: myjni.c multistuff.settings.h
	gcc-4.2 $(STUFF) -c -o $@ $<

%.o: %.S
	gcc-4.2 $(STUFF) -c -o $@ $<


OBJS := food.o np.o anp.o multistuff.o common.o myjni.o classes.o fixups/cfixups.o fixups/cppfixups.o fixups/rthread.o fixups/sfixups.o ../rpc/food_rpc1.o
food: $(OBJS)
	g++-4.2 $(STUFF) -o food $(OBJS) -F$(SDK)/System/Library/Frameworks -F$(SDK)/System/Library/PrivateFrameworks -framework CoreGraphics -framework IOSurface -framework CoreFoundation -framework CoreText -framework AudioToolbox -framework GraphicsServices -image_base 0x08000000

clean:
	rm -f *.o fixups/*.o food
