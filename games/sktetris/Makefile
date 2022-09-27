CXX = skm clang++
src = $(wildcard src/*.cpp)

LDFLAGS = -O3
OSFLAGS = 

sktetris: $(src)
	$(CXX) -o $@ $^ $(LDFLAGS) $(OSFLAGS)
