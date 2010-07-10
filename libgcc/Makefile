include ../env.mk

libgccstuff.dylib:
	ar x /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS3.2.sdk/usr/lib/gcc/arm-apple-darwin10/4.2.1/libgcc.a
	python fix.py
	python fix2.py
	$(GCC) -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS3.2.sdk/ -arch armv7 -dynamiclib -exported_symbols_list symbols -o libgccstuff.dylib *.o 
clean:	
	rm -f *.o libgccstuff.dylib
