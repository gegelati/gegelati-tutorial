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


# Open the files
cppInputFile = open("./src/training/pendulum_wrapper.cpp","r")
cppEmptyOutputFile = open("./src/training/pendulum_wrapper_empty.cpp", "w")
cppSolutionOutputFile = open("./src/training/pendulum_wrapper_solution.cpp", "w")
hInputFile = open("./src/training/pendulum_wrapper.h","r")
hEmptyOutputFile = open("./src/training/pendulum_wrapper_empty.h", "w")
hSolutionOutputFile = open("./src/training/pendulum_wrapper_solution.h", "w")
txtInputCMakeListsFile = open("./CMakeLists.txt", "r")
txtEmptyCMakeListsFile = open("./CMakeLists_empty.txt", "w")
txtSolutionCMakeListsFile = open("./CMakeLists_solution.txt", "w")

if(not cppInputFile or not cppEmptyOutputFile or not hInputFile or not hEmptyOutputFile or not txtEmptyCMakeListsFile or not txtInputCMakeListsFile or not txtSolutionCMakeListsFile):
    exit


## Filter header file
filterSolution(hInputFile, hEmptyOutputFile, False, "SOLUTION.*")
filterSolution(hInputFile, hSolutionOutputFile, True, "SOLUTION.*")

## Filter cpp file
filterSolution(cppInputFile, cppEmptyOutputFile, False, "SOLUTION.*")
filterSolution(cppInputFile, cppSolutionOutputFile, True, "SOLUTION.*")

## Filter CMakeLists file
filterSolution(txtInputCMakeListsFile, txtEmptyCMakeListsFile, False, "SOLUTION.*")
filterSolution(txtInputCMakeListsFile, txtSolutionCMakeListsFile, True, "SOLUTION.*")

# Close files
cppInputFile.close()
cppEmptyOutputFile.close()
cppSolutionOutputFile.close()
hInputFile.close()
hEmptyOutputFile.close()
hSolutionOutputFile.close()
txtSolutionCMakeListsFile.close()
txtInputCMakeListsFile.close()
txtEmptyCMakeListsFile.close()
