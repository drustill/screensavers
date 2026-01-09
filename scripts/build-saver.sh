#!/bin/bash

name=$1
view_class=$2
display_name=${3:-$name}
outdir=savers/$display_name.saver

mkdir -p "$outdir/Contents/MacOS"
mkdir -p "$outdir/Contents/Resources"

clang++ -bundle -o "$outdir/Contents/MacOS/$name" \
    "src/$name/macos.mm" "src/$name/$name.cpp" "src/glad.cpp" \
    -std=c++17 -Iinclude -framework Cocoa -framework OpenGL -framework ScreenSaver -fobjc-arc -DGL_SILENCE_DEPRECATION

cp "src/$name/$name.vs" "src/$name/$name.fs" "$outdir/Contents/Resources/"

cat > "$outdir/Contents/Info.plist" << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key><string>$display_name</string>
    <key>CFBundleIdentifier</key><string>com.screensaver.$name</string>
    <key>CFBundleInfoDictionaryVersion</key><string>6.0</string>
    <key>CFBundleName</key><string>$display_name</string>
    <key>CFBundlePackageType</key><string>BNDL</string>
    <key>CFBundleShortVersionString</key><string>1.0</string>
    <key>CFBundleVersion</key><string>1</string>
    <key>NSPrincipalClass</key><string>$view_class</string>
</dict>
</plist>
EOF

echo "Built $display_name.saver"