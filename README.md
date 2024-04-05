timer - a simple cli stopwatch and timer
===

The goal is to have a simple and lightweight interactive timer/stopwatch in the terminal with no external dependencies. Simplicity is favoured over high precision, and the feature set will be built around my day-to-day use of this tool.

```
$ ./timer --help
Usage: timer [OPTIONS] [TIME]
Simple command line stopwatch and timer.
Behaves like a stopwatch if no TIME is given.

  -a, --art                     use ASCII art for display
  -c, --cmd=CMD                 run the given command in sh at program exit
  -f, --fullform                use hh:mm:ss:lll format
  -R, --resolution=h|m|s|l      set hour/minute/second/millisecond resolution
  -r, --resume                  resume last session(!)
      --help                    print this help message and exit
      --version                 print version and exit

TIME is specified as: ([[:uint:]][hmsl])+

Examples:
  timer -Rlf    stopwatch with 1ms resolution displayed in full format
  timer 1m30s   one and a half minute timer with 1s resolution
  timer -a      stopwatch with ASCII art formatting and auto color

timer 0.9.3-alpha4
Written by Raul Cotar, licensed under GPLv3.
Source, documentation and support: <https://github.com/RaulCotar/timer>
```

## TODO
- countdown mode
- interactive commands
- persistent session

## Buillding
Simply run `$ make`.
You will need a compiler that supports the C23 language standard.
