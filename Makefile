
bin/main: main.cc
	mkdir -p bin
	g++ --std=c++11 -o bin/main main.cc

.PHONY: run
run: bin/main
	bin/main
