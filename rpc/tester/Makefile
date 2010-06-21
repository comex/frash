all:
	python ../PROTOCOLBUFFERS.py tester.proto
	gcc -g3 -m32 -o tester1 tester1.c tester_rpc1.c -framework CoreFoundation
	gcc -g3 -m32 -o tester2 tester2.c tester_rpc2.c -framework CoreFoundation
