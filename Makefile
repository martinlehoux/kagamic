debug:
	cd Debug; \
	cmake -DCMAKE_BUILD_TYPE=Debug ..; \
	make;

release:
	cd Release; \
	cmake -DCMAKE_BUILD_TYPE=Release ..; \
	make;
