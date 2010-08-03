include ../../env.mk
Plugin.dylib: PluginFlashView.o server.o food_rpc2.o
	$(GCC) -dynamiclib -o $@ $^ -framework CoreFoundation -framework Foundation  -framework CoreGraphics -framework UIKit -framework IOSurface
%.o: ../Classes/%.[cm]
	$(GCC) -std=gnu99 -c -o $@ $< -I../Classes -I../IOSurface -I.. -Dstream_t=int
%.o: ../%.[cm]
	$(GCC) -std=gnu99 -c -o $@ $< -I../Classes -I../IOSurface -I.. -Dstream_t=int
clean:
	rm -f *.o Plugin.dylib
install:
	scp Plugin.dylib root@ipad:'/System/Library/Internet\ Plug-Ins/Frash.webplugin/Plugin.dylib'
