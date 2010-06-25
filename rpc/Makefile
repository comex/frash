all:
	python PROTOCOLBUFFERS.py food.proto
	gcc-4.2 -arch armv7 -O2 -g3 --sysroot /var/sdk -Dstream_t=int -c food_rpc[12].c
	cp food_rpc2.h food_rpc2.c ../Player2/
clean:
	rm *.[cho]
