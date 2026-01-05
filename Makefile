CXXFLAGS = -std=c++17 -Iinclude -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit

BINDIR = bin
SAVERDIR = savers

all: plasma particle quads clover

savers: plasma.saver particle.saver quads.saver clover.saver

$(BINDIR):
	mkdir -p $(BINDIR)

$(SAVERDIR):
	mkdir -p $(SAVERDIR)

new:
	@./scripts/new-screensaver.sh

plasma: $(BINDIR) src/plasma/plasma.cpp src/plasma/main.cpp
	clang++ -o $(BINDIR)/plasma src/plasma/main.cpp src/plasma/plasma.cpp src/glad.cpp $(CXXFLAGS)

particle: $(BINDIR) src/particle/particle.cpp src/particle/main.cpp
	clang++ -o $(BINDIR)/particle src/particle/main.cpp src/particle/particle.cpp src/glad.cpp $(CXXFLAGS)

quads: $(BINDIR) src/quads/quads.cpp src/quads/main.cpp
	clang++ -o $(BINDIR)/quads src/quads/main.cpp src/quads/quads.cpp src/glad.cpp $(CXXFLAGS)

clover: $(BINDIR) src/clover/clover.cpp src/clover/main.cpp
	clang++ -o $(BINDIR)/clover src/clover/main.cpp src/clover/clover.cpp src/glad.cpp $(CXXFLAGS)

windfield: $(BINDIR) src/windfield/windfield.cpp src/windfield/main.cpp
	clang++ -o $(BINDIR)/windfield src/windfield/main.cpp src/windfield/windfield.cpp src/glad.cpp $(CXXFLAGS)

plasma.saver:
	./scripts/build-saver.sh plasma PlasmaSaverView

particle.saver:
	./scripts/build-saver.sh particle ParticleSaverView

quads.saver:
	./scripts/build-saver.sh quads QuadsSaverView

clover.saver:
	./scripts/build-saver.sh clover CloverSaverView

install: savers
	@echo "Installing to ~/Library/Screen Savers/..."
	@mkdir -p ~/Library/Screen\ Savers
	@cp -R $(SAVERDIR)/*.saver ~/Library/Screen\ Savers/

uninstall:
	@echo "Uninstalling from ~/Library/Screen Savers/..."
	@rm -rf ~/Library/Screen\ Savers/plasma.saver
	@rm -rf ~/Library/Screen\ Savers/particle.saver
	@rm -rf ~/Library/Screen\ Savers/quads.saver
	@rm -rf ~/Library/Screen\ Savers/clover.saver

clean:
	rm -rf $(BINDIR)/*
	rm -rf $(SAVERDIR)

.PHONY: all new savers install uninstall clean plasma particle quads clover windfield plasma.saver particle.saver quads.saver clover.saver
