debug:
	mkdir -p Debug
	cmake -DCMAKE_BUILD_TYPE=Debug -B Debug
	cmake --build Debug

release:
	mkdir -p Release
	cmake -DCMAKE_BUILD_TYPE=Release -B Release
	cmake --build Release

profile:
	mkdir -p Profile
	cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -B Profile
	cmake --build Profile
	valgrind --tool=callgrind --dump-instr=yes --callgrind-out-file=callgrind.out ./Profile/main 100MB.json 1
	kcachegrind callgrind.out

test:
	fish generate.fish
	make debug
	./Debug/test

format:
	clang-format -i src/*.c include/*.h tests/*.c *.c
