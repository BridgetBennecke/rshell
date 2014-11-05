# rShell

## Attributes

rShell can run multiple commands.
* Commands can be connected by ```;```,```||```,or ```&&```.
* Prints out prompt as ```user@hostname$```
* Anything typed after a ```#``` is considered a comment and not run

## Known Bugs

* the ```cd``` command does not work
* if multiple connectors are used on the same line, any connectors different from the first one will be seen as arguments
* quotation marks and parenthesis are not seen differently from other arguments
* adding a single connector to the end of a lone command without parameters will cause the execution to fail
* running exit with a connector and no other commands will cause exit to fail
* seems to only successfully executes commands in /bin
* a line cannot exclusively be a comment, else ```#``` is seen as a command

