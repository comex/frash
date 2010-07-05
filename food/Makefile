export SDK := /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS3.2.sdk/

all: food food_wrapper

STUFF = -mthumb -Wreturn-type -Wall -DHAVE_MACOSX_IPC -DOS_PATH_SEPARATOR="'/'" -DHAVE_PTHREADS -DHAVE_POSIX_FILEMAP -DHAVE_LITTLE_ENDIAN -I. -I../rpc -I../utils -I../utils/libcutils -isysroot $(SDK) -arch armv7 -ffixed-r9 -Wno-uninitialized -g3 -O2
DEBUG = 1

#ifeq "$(DEBUG)" "1"
#STUFF += -g3
#else
#STUFF += -O2
#endif

%.o: %.cpp common.h ../rpc/food_rpc1.h
	g++-4.2 $(STUFF) -c -o $@ $<

%.o: %.c common.h ../rpc/food_rpc1.h
	gcc-4.2 $(STUFF) -c -o $@ $<

multistuff.c: multistuff.py
	python multistuff.py

multistuff.settings.h: multistuff.py
	python multistuff.py

myjni.o: myjni.c multistuff.settings.h
	gcc-4.2 $(STUFF) -c -o $@ $<

%.o: %.S
	gcc-4.2 $(STUFF) -c -o $@ $<


OBJS := food.o np.o anp.o neon.o multistuff.o common.o myjni.o classes.o fixups/cfixups.o fixups/cppfixups.o fixups/rthread.o fixups/sfixups.o
food: ../rpc/food_rpc1.h $(OBJS)
	g++-4.2 $(STUFF) -o food $(OBJS) ../rpc/food_rpc1.o -F$(SDK)/System/Library/Frameworks -F$(SDK)/System/Library/PrivateFrameworks -framework CoreGraphics -framework IOSurface -framework CoreFoundation -framework CoreText -framework AudioToolbox -framework GraphicsServices -image_base 0x08000000
	ldid -S food

food_wrapper: food_wrapper.c
	gcc-4.2 $(STUFF) -o food_wrapper food_wrapper.c

clean:
	rm -f *.o fixups/*.o food
