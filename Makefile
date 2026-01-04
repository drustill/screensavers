CXXFLAGS = -std=c++17 -Iinclude -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit
SAVER_CXXFLAGS = -std=c++17 -Iinclude -framework Cocoa -framework OpenGL -framework ScreenSaver -fobjc-arc -DGL_SILENCE_DEPRECATION

BINDIR = bin
SAVERDIR = savers

all: plasma particle quads clover

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

savers: plasma.saver particle.saver quads.saver clover.saver

define SAVER_TEMPLATE
$(1).saver: $$(SAVERDIR) src/$(1)/$(1).cpp src/$(1)/macos.mm
	@echo "Building $(1).saver..."
	@mkdir -p $$(SAVERDIR)/$(1).saver/Contents/MacOS
	@mkdir -p $$(SAVERDIR)/$(1).saver/Contents/Resources
	clang++ -bundle -o $$(SAVERDIR)/$(1).saver/Contents/MacOS/$(1) \
		src/$(1)/macos.mm src/$(1)/$(1).cpp src/glad.cpp \
		$$(SAVER_CXXFLAGS)
	@cp src/$(1)/$(1).vs src/$(1)/$(1).fs $$(SAVERDIR)/$(1).saver/Contents/Resources/
	@echo '<?xml version="1.0" encoding="UTF-8"?>' > $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '<plist version="1.0">' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '<dict>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '    <key>CFBundleExecutable</key>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '    <string>$(1)</string>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '    <key>CFBundleIdentifier</key>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '    <string>com.screensaver.$(1)</string>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '    <key>CFBundleInfoDictionaryVersion</key>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '    <string>6.0</string>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '    <key>CFBundleName</key>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '    <string>$(1)</string>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '    <key>CFBundlePackageType</key>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '    <string>BNDL</string>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '    <key>CFBundleShortVersionString</key>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '    <string>1.0</string>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '    <key>CFBundleVersion</key>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '    <string>1</string>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '    <key>NSPrincipalClass</key>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '    <string>$(2)</string>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '</dict>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo '</plist>' >> $$(SAVERDIR)/$(1).saver/Contents/Info.plist
	@echo "Built $(1).saver"
endef

$(eval $(call SAVER_TEMPLATE,plasma,PlasmaSaverView))
$(eval $(call SAVER_TEMPLATE,particle,ParticleSaverView))
$(eval $(call SAVER_TEMPLATE,quads,QuadsSaverView))
$(eval $(call SAVER_TEMPLATE,clover,CloverSaverView))

install: savers
	@echo "Installing screensavers to ~/Library/Screen Savers/..."
	@mkdir -p ~/Library/Screen\ Savers
	@cp -R $(SAVERDIR)/*.saver ~/Library/Screen\ Savers/
	@echo "Done! Open System Settings > Screen Saver to select one."

uninstall:
	@echo "Removing screensavers from ~/Library/Screen Savers/..."
	@rm -rf ~/Library/Screen\ Savers/plasma.saver
	@rm -rf ~/Library/Screen\ Savers/particle.saver
	@rm -rf ~/Library/Screen\ Savers/quads.saver
	@rm -rf ~/Library/Screen\ Savers/clover.saver
	@echo "Done!"

clean:
	rm -rf $(BINDIR)/*
	rm -rf $(SAVERDIR)

.PHONY: all new savers install uninstall clean plasma particle quads clover plasma.saver particle.saver quads.saver clover.saver
