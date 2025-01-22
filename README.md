# Tales of Zestiria 60FPS Patch

Patches the game to run at 60 FPS without slow motion.

## Installing

1. Copy "xinput1_3.dll" to the root directory of the game, where "Tales of Zestiria.exe" is located.
2. (Steam Deck Only) Add `WINEDLLOVERRIDES="xinput1_3=n,b" %command%` to the game launch options.

## Building from source

On Linux:

```shell
# First you should install mingw-w64 (https://www.mingw-w64.org/), gcc and make
# On Ubuntu it would be:
sudo apt install make mingw-w64 gcc

# Clone this repository:
git clone https://github.com/NishiyamaPedro/tz-fps-patch

# Build the source code:
make

# Now a file named `xinput1_3.dll` should appear in the repository
# Copy it to your Tales of Zestiria's root directory
```
