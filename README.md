timer - a simple cli stopwatch and timer
===

The goal is to have a simple and lightweight interactive timer/stopwatch in the terminal with no external dependencies. Simplicity is favoured over high precision, and the feature set will be built around my day-to-day use of this tool.

```
$ ./timer --help
Usage: timer [OPTIONS] [TIME]
Simple command line stopwatch and timer.
Behaves like a stopwatch if no TIME is given.

  -a, --art                 use ASCII art for display
  -c, --endcmd=CMD		      run the given command in sh at program exit
  -f, --fullform		        use hh:mm:ss:lll format
  -n, --no-interact		      run in non-interactive mode
  -R, --resolution=RES		  set timer resolution (default: 1s)
  -r, --resume			        resume last session(!)
      --help		  	        print this help message and exit
      --version			        print version and exit

TIME and RES are specified as: ([[:uint:]][hmsl])+

Examples:
  timer -Rlf    stopwatch with 1ms resolution displayed in full format
  timer 1m30s   one and a half minute timer with 1s resolution
  timer -a      stopwatch with ASCII art formatting and auto color

timer 0.9.4-alpha5
Written by Raul Cotar, licensed under GPLv3.
Source, documentation and support: <https://github.com/RaulCotar/timer>
```

## Use cases
- I will mostly use this as a stopwatch with minute granularity to time the different ways I spend time in front of the computer. The ability to suspend the process and resume it only when I wanto to check the time comes in handy.
- Another use case is showing off the ASCII art rendering with the full format - for the cool factor.
- One possibility I want to explorte is the --cmd flag, maybe that that will come in handy some time.

## TODO
- interactive commands
- persistent session

## Buillding
Simply run `$ make`.
You will need a compiler that supports the C23 language standard.
