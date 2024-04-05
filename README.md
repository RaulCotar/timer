timer - a simple cli stopwatch and timer
===

```
$ ./timer --help
Usage: timer [OPTIONS] [TIME]
Simple command line stopwatch and timer.(!)
Behaves like a stopwatch if no TIME is given.

  -a, --art                     use ASCII art for display(!)
  -f, --fullform                use hh:mm:ss:lll format
  -R, --resolution=[h|m|s|l]    set hour/minute/second/millisecond resolution
  -r, --resume                  resume last session(!)
      --help                    print this help message and exit
      --version                 print version and exit

TIME is specified as:
  %d[h|m|s|l](:...)* with strictly increasing order of precision

Examples:
  timer -Rlf    stopwatch with 1ms resolution displayed in full format
  timer 1m30s   one and a half minute timer with 1s resolution
  timer -a      stopwatch with ASCII art formatting and auto color

timer 0.9.1-alpha2
Written by Raul Cotar, licensed under GPLv3.
Source, documentation and support: <https://github.com/RaulCotar/timer>
```

## TODO
- `-o OFS`
- `TIME`
- interactive commands

## Quirks
- some things are not implemented yet
- `timer` doesn't clear any styling that it does not set itself. This means that you can set some styling yourself before calling `timer`, for example: `echo -ne '\x1b[1;7;32m' && timer` will result in the timer displayed in bold on a green background.
