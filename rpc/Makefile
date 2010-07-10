include ../env.mk

all:
	python PROTOCOLBUFFERS.py food.proto
	$(GCC) -O2 -g3 --sysroot $(SDK) -Dstream_t=int -c food_rpc[12].c
	cp food_rpc2.h food_rpc2.c ../Player2/
clean:
	rm *.[cho]
