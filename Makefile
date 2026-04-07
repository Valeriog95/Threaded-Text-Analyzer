.PHONY: all clean test

all:
	mkdir -p build && cd build && cmake .. && make

clean:
	rm -rf build bin
	rm -rf build docs/html

doc:
	@echo "Generating Doxygen documentation..."
	doxygen Doxyfile
	@echo "Documentation ready in docs/html/index.html"
	
test:
	cd build && make test

run:
	./build/bin/ThreadedTextAnalyzer $(ARGS)