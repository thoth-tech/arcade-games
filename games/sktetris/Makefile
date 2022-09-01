CXX = skm clang++
src = $(wildcard src/*.cpp)

LDFLAGS = -O3
OSFLAGS = 

sktetris: $(src)
	@mkdir -p bin
	$(CXX) -o bin/$@ $^ $(LDFLAGS) $(OSFLAGS)
