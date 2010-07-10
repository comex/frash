include ../env.mk

all: food food_wrapper

STUFF = -mthumb -Wreturn-type -Wall -DHAVE_MACOSX_IPC -DOS_PATH_SEPARATOR="'/'" -DHAVE_PTHREADS -DHAVE_POSIX_FILEMAP -DHAVE_LITTLE_ENDIAN -I. -I../rpc -I../utils -I../utils/libcutils -ffixed-r9 -Wno-uninitialized -g3 -O2

%.o: %.cpp common.h ../rpc/food_rpc1.h
	$(GXX) $(STUFF) -c -o $@ $<

%.o: %.c common.h ../rpc/food_rpc1.h
	$(GCC) $(STUFF) -c -o $@ $<

multistuff.c: multistuff.py
	python multistuff.py

multistuff.settings.h: multistuff.py
	python multistuff.py

myjni.o: myjni.c multistuff.settings.h
	$(GCC) $(STUFF) -c -o $@ $<

ifdef IPHONE_GCC
# current iphone-gcc doesn't support NEON stuff. :(
neon.o: neon.S
	cp prebuilt-for-iphone-gcc/neon.o neon.o
else
neon.o: neon.S
	$(GCC) $(STUFF) -c -o $@ $<
endif

%.o: %.S
	$(GCC) $(STUFF) -c -o $@ $<


OBJS := food.o np.o anp.o  multistuff.o common.o myjni.o classes.o fixups/cfixups.o fixups/cppfixups.o fixups/rthread.o fixups/sfixups.o neon.o
food: ../rpc/food_rpc1.h $(OBJS)
	$(GXX) $(STUFF) -o food $(OBJS) ../rpc/food_rpc1.o -F$(SDK)/System/Library/Frameworks -F$(SDK)/System/Library/PrivateFrameworks -framework CoreGraphics -framework IOSurface -framework CoreFoundation -framework CoreText -framework AudioToolbox -framework GraphicsServices -image_base 0x08000000
	ldid -S food

food_wrapper: food_wrapper.c
	$(GCC) $(STUFF) -o food_wrapper food_wrapper.c
	ldid -S food_wrapper

clean:
	rm -f *.o fixups/*.o food
