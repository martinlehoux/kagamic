debug:
	mkdir -p Debug; \
	cd Debug; \
	cmake -DCMAKE_BUILD_TYPE=Debug ..; \
	make;

release:
	mkdir -p Release; \
	cd Release; \
	cmake -DCMAKE_BUILD_TYPE=Release ..; \
	make;

profile: release
	valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./Release/main
	kcachegrind callgrind.out

test:
	fish generate.fish;
	make debug;
	./Debug/test;

format:
	clang-format -i src/*.c include/*.h tests/*.c *.c
