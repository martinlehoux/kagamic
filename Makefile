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

test:
	fish generate.fish;
	make debug;
	./Debug/test;

format:
	clang-format -i *.c *.h
