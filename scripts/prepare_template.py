# This script prepares pendulum_wrapper.h/cpp files for the tutorial.
#
# Author: K. Desnos
# License: CeCILL-C

import re


# Function filtering the solution out of the input file.
# also filters double empty lines that may result from filtering.
def filterSolution(inputFile, outputFile, keepSolution, pattern):
    # rewind input file
    inputFile.seek(0)

    # Scan lines
    isSolution = False
    isTemplate = False
    emptyLine = False
    for line in inputFile:
        # Check if the line is the #define
        if(re.match(rf'.*#define SOLUTION.*\n', line)):
            continue # skip the line

        # Check if the line starts a solution block
        if(re.match(rf'.*#ifdef {pattern}\s*\n', line)):
            isSolution = True
            continue # skip the line

        # Check if the line start a template block
        if(isSolution and re.match(r'.*#else.*\n', line)):
            isSolution = False
            isTemplate = True
            continue # skip the line

        # Check if the line ends a block
        if((isSolution or isTemplate ) and re.match(rf'.*#endif // {pattern}\s*\n', line)):
            isSolution = False
            isTemplate = False
            continue # skip the line

        printLine = False
        if keepSolution:
            if isSolution or (not isSolution and not isTemplate):
                printLine = True
        else:
            if isTemplate or (not isSolution and not isTemplate):
                printLine = True

        if printLine:
            if(re.match(r'\s*\n', line)):
                if emptyLine:
                    continue # skipLine
                else:
                    emptyLine = True
            else:
                emptyLine = False
            outputFile.write(line)


# Files to filter
files = [
    ["./src/training/pendulum_wrapper.cpp", "./src/training/pendulum_wrapper_empty.cpp", "./src/training/pendulum_wrapper_solution.cpp"],
    ["./src/training/pendulum_wrapper.h", "./src/training/pendulum_wrapper_empty.h", "./src/training/pendulum_wrapper_solution.h"],
    ["./CMakeLists.txt", "./CMakeLists_empty.txt", "./CMakeLists_solution.txt"]
]

# Prepare files
for fileSet in files:
    inputFilePath = fileSet[0]
    emptyOutputFilePath = fileSet[1]
    solutionOutputFilePath = fileSet[2]

    # Open the files
    inputFile = open(inputFilePath, "r")
    emptyOutputFile = open(emptyOutputFilePath, "w")
    solutionOutputFile = open(solutionOutputFilePath, "w")

    if(not inputFile or not emptyOutputFile or not solutionOutputFile):
        exit

    # Filter empty version
    filterSolution(inputFile, emptyOutputFile, False, "SOLUTION.*")
    # Filter solution version
    filterSolution(inputFile, solutionOutputFile, True, "SOLUTION.*")

    # Close files
    inputFile.close()
    emptyOutputFile.close()
    solutionOutputFile.close()
