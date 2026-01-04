#!/bin/bash

cd "$(dirname "$0")/.."

echo "Name of screensaver:"
read name

if [ -z "$name" ]; then
  echo "Invalid name. Cannot be empty. Exiting."
  exit 1
fi

mkdir src/$name
touch src/$name/$name.cpp src/$name/$name.h src/$name/$name.vs src/$name/$name.fs src/$name/macos.mm src/$name/main.cpp
echo "Initialized $name"
