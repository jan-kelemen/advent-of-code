import strutils

var frequency = 0
var line = ""
while readLine(stdin, line):
    let shift = parseInt(line)
    frequency += shift;
echo frequency
