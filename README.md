# Screensaver Playground

## GLFW

```bash
# clover is the name of the screensaver
make clover
./bin/clover
```

Press **Escape** to exit.

## macOS

To install and test through system settings (like xscreensaver)

```bash
make savers

# Installs to ~/Library/Screen Savers/
make install

# You can also just take one
cp -R savers/clover.saver ~/Library/Screen Savers/
```

Helpful for cleanup

```bash
make uninstall
```
