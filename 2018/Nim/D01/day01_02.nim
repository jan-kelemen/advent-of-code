import strutils, intsets

proc readFrequencies() : seq[int] =
    result = @[]
    var line = ""
    while readLine(stdin, line):
        let shift = parseInt(line)
        add(result, shift)

let frequencies = readFrequencies()
var changes = initIntSet()
var currentFrequency = 0
var index = 0

while not containsOrIncl(changes, currentFrequency):
    currentFrequency += frequencies[index]
    index = (index + 1) mod len(frequencies)
echo currentFrequency
