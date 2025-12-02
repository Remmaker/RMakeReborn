# RMake Reborn

A fast, readable, **no-bullshit** build system for personal C/C++ projects.

No CMakeLists hell. No 500-line Makefiles.  
Just a clean `.rm` file you can read (and edit) in 30 seconds.

MIT licensed — take it, break it, ship it, sell it, I don't care.  
Just keep the license and don't blame me if it eats your cat.

### Example · conf.rm
```ini
#   <- Comment token, no multiline token, just use line by line commentary.
//  <- Also comment
;   <- Also also comment

VERBOSE=y <- Show the stderr & stdout of the [run] section
EXIT_CODE=y <- Show the exit code of the [run] section

build 
{
    compiler=g++
    flags=-std=c++17 -Wall -Werror -Wextra -ggdb
    src=src/*.cpp
    include=src/include
    target=progtest.exe
}

run
{
    interpreter=python
    prescript=pretest.py
    run=progtest.exe
    postscript=testpost.py
}
```

##### See `example/`

# Usage 
```text
rmake                    # build (auto-finds *.rm)
rmake mygame.rm | mygame # build a specific file
rmake run                # run the [run] section (no rebuild)
rmake watch              # hot-reload on save (WIP)
rmake help               # this message
```
Why this exists
I got tired of suffering with Make and CMake for my personal projects.
So I rewrote my old C nightmare (see `legacy/`) in Rust.
Now it works, it's fast, and it doesn't make me cry.

*— Remmaker*