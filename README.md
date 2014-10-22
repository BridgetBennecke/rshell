# rShell

## Attributes

rShell can run commands as a shell, even multiple ones. Commands can be connected by ''';''','''||''',or '''&&'''.
* Maximum of 10 arguments may be used per command
* Prints out prompt as '''user@hostname$'''

## Known Bugs

* the '''cd''' command does not work
* if multiple connectors are used on the same line, any connectors different from the first one will be seen as arguments
* quotation marks and parenthesis are not seen differently from other arguments

