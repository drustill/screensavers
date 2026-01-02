CXXFLAGS = -Iinclude -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit

all: plasma particle quads clover

new:
	@./scripts/new-screensaver.sh

plasma: src/plasma/plasma.cpp
	clang++ -o bin/plasma src/plasma/plasma.cpp src/glad.cpp $(CXXFLAGS)

particle: src/particle/particle.cpp
	clang++ -o bin/particle src/particle/particle.cpp src/glad.cpp $(CXXFLAGS)

quads: src/quads/quads.cpp
	clang++ -o bin/quads src/quads/quads.cpp src/glad.cpp $(CXXFLAGS)

clover: src/clover/clover.cpp
	clang++ -o bin/clover src/clover/clover.cpp src/glad.cpp $(CXXFLAGS)

clean:
	rm -f bin/*
