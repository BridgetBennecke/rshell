# rShell

## Attributes

rShell can run commands as a shell, even multiple ones. Commands can be connected by ';','||',or '&&'.
* Maximum of 10 arguments may be used per command

## Known Bugs

* Connectors '||' and '&&' serve the same as ';'
* When '||' or '&&' is used a segmentation fault occurs
* the 'cd' command does not work
