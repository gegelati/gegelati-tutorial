# This script prepares pendulum_wrapper.h/cpp files for the tutorial.
#
# Author: K. Desnos
# License: CeCILL-C

import re
import io


# Function filtering the solution out of the input file.
# also filters double empty lines that may result from filtering.
def filterSolution(inputFile, outputFile, keepSolution, patterns):
    # Start with the original input file
    currentInput = inputFile

    # Apply each pattern sequentially
    for pattern in patterns:
        # Use an in-memory buffer to store intermediate results
        tempOutput = io.StringIO()
        currentInput.seek(0)

        # Stack to track active blocks
        blockStack = []
        inElseBlock = False  # Flag to track if we are in an `#else` block
        emptyLine = False

        for line in currentInput:
            # Check if the line is the #define
            if re.match(rf'.*#define SOLUTION.*\n', line):
                continue  # skip the line

            # Check if the line starts a solution block
            matchIfdef = re.match(rf'.*#ifdef ({pattern})\s*\n', line)
            if matchIfdef and not inElseBlock:
                blockStack.append(matchIfdef.group(1))  # Push the matched pattern onto the stack
                inElseBlock = False  # Reset the `else` flag
                continue  # skip the line

            # Check if the line starts a template block
            if blockStack and re.match(rf'.*#else // ({pattern})\s*\n', line):
                inElseBlock = True  # Mark that we are in the `else` section
                continue  # skip the line

            # Check if the line ends a block
            matchEndif = re.match(rf'.*#endif // ({pattern})\s*\n', line)
            if blockStack and matchEndif and blockStack[-1] == matchEndif.group(1):
                blockStack.pop()  # Pop the matched block
                inElseBlock = False  # Reset the `else` flag
                continue  # skip the line

            printLine = False
            if keepSolution:
                if not blockStack or not inElseBlock:
                    printLine = True
            else:
                if not blockStack or inElseBlock:
                    printLine = True

            if printLine:
                if re.match(r'\s*\n', line):
                    if emptyLine:
                        continue  # skipLine
                    else:
                        emptyLine = True
                else:
                    emptyLine = False
                tempOutput.write(line)

        # Replace the current input with the output of this iteration
        tempOutput.seek(0)
        currentInput = tempOutput

    # Write the final result to the output file
    currentInput.seek(0)
    for line in currentInput:
        outputFile.write(line)


# Files to filter
# Each entry: [inputPath, [(outputPath, patternsToRemove, patternsToKeep), ...]]
files = [
    ["./src/training/pendulum_wrapper.cpp", [
        ["./src/training/pendulum_wrapper_empty.cpp", ["SOLUTION","SOLUTION_.*"], []],
        ["./src/training/pendulum_wrapper_solution.cpp", ["SOLUTION_.*"], ["SOLUTION"]],
        ["./src/training/pendulum_wrapper_strengthening.cpp", ["SOLUTION_PARALLEL"], ["SOLUTION(_STRENGTHENING)*"]],
        ["./src/training/pendulum_wrapper_parallel.cpp", [], ["SOLUTION_(PARALLEL|STRENGTHENING)", "SOLUTION"]],
    ]],
    ["./src/training/pendulum_wrapper.h", [
        ["./src/training/pendulum_wrapper_empty.h", ["SOLUTION.*"], []],
        ["./src/training/pendulum_wrapper_solution.h", ["SOLUTION_.*"], ["SOLUTION.*"]],
        ["./src/training/pendulum_wrapper_strengthening.h", ["SOLUTION_PARALLEL"], ["SOLUTION(_STRENGTHENING)*"]],
        ["./src/training/pendulum_wrapper_parallel.h", [], ["SOLUTION_(PARALLEL|STRENGTHENING)", "SOLUTION"]],
    ]],
    ["./CMakeLists.txt", [
        ["./CMakeLists_empty.txt", ["SOLUTION.*"], []],
        ["./CMakeLists_inference.txt", [], ["SOLUTION_INFERENCE"]],
    ]],
    ["./src/training/main-training.cpp", [
     ["./src/training/main-training_empty.cpp", ["SOLUTION.*"], []],
     ["./src/training/main-training_parallel.cpp", [], ["SOLUTION_PARALLEL"]],
    ]],
    ["./params.json", [
     ["./params_empty.json", ["SOLUTION.*"], []],
     ["./params_strengthening.json", ["SOLUTION_PARALLEL"], ["SOLUTION_STRENGTHENING"]],
    ]],
]

# Prepare files
for fileSet in files:
    inputFilePath = fileSet[0]
    outputs = fileSet[1]

    inputFile = open(inputFilePath, "r")
    if not inputFile:
        continue

    for out in outputs:
        outputPath, patternsRemove, patternsKeep = out
        outputFile = open(outputPath, "w")
        if patternsRemove and not patternsKeep:
            filterSolution(inputFile, outputFile, False, patternsRemove)
        elif patternsKeep and not patternsRemove:
            filterSolution(inputFile, outputFile, True, patternsKeep)
        elif patternsKeep and patternsRemove:
            # Remove first into an in-memory buffer, then keep from that buffer
            temp = io.StringIO()
            filterSolution(inputFile, temp, False, patternsRemove)
            filterSolution(temp, outputFile, True, patternsKeep)
            temp.close()
        else:
            # No pattern provided: copy file as-is
            inputFile.seek(0)
            for line in inputFile:
                outputFile.write(line)
        outputFile.close()

    inputFile.close()
