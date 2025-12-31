CXXFLAGS = -Iinclude -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit

all: plasma

plasma: src/plasma/plasma.cpp src/glad.cpp
	clang++ -o bin/plasma src/plasma/plasma.cpp src/glad.cpp $(CXXFLAGS)

clean:
	rm -f bin/*
