# This script prepares pendulum_wrapper.h/cpp files for the tutorial.
#
# Author: K. Desnos
# License: CeCILL-C

import re


# Function filtering the solution out of the input file.
# also filters double empty lines that may result from filtering.
def filterSolution(inputFile, outputEmptyFile, outputSolutionFile):
    
    # Scan lines
    isSolution = False
    isTemplate = False
    emptyEmptyLine = False
    emptySolutionLine = False
    for line in inputFile:
        # Check if the line is the #define
        if(re.match(r'.*#define SOLUTION.*\n', line)):
            continue # skip the line

        # Check if the line starts a solution block
        if(re.match(r'.*#ifdef SOLUTION.*\n', line)):
            isSolution = True
            continue # skip the line
        
        # Check if the line start a template block
        if(isSolution and re.match(r'.*#else.*\n', line)):
            isSolution = False
            isTemplate = True
            continue # skip the line

        # Check if the line start a template block
        if((isSolution or isTemplate ) and re.match(r'.*#endif // SOLUTION.*\n', line)):
            isSolution = False
            isTemplate = False
            continue # skip the line

        printEmptyLine = False
        printSolutionLine = False

        if(isTemplate):
            printEmptyLine=True
        
        if(isSolution):
            printSolutionLine = True

        if(not isSolution and not isTemplate):
            printEmptyLine=True
            printSolutionLine = True

        # Print line in empty file
        if(printEmptyLine):
            if(re.match(r'\s*\n', line)):
                if(emptyEmptyLine):
                    continue # skipLine
                else:
                    emptyEmptyLine=True
            else:
                emptyEmptyLine=False

            outputEmptyFile.write(line)

        # Print line in solution file
        if(printSolutionLine):
            if(re.match(r'\s*\n', line)):
                if(emptySolutionLine):
                    continue # skipLine
                else:
                    emptySolutionLine=True
            else:
                emptySolutionLine=False

            outputSolutionFile.write(line)


# Open the files
cppInputFile = open("./src/training/pendulum_wrapper.cpp","r")
cppEmptyOutputFile = open("./src/training/pendulum_wrapper_empty.cpp", "w")
cppSolutionOutputFile = open("./src/training/pendulum_wrapper_solution.cpp", "w")
hInputFile = open("./src/training/pendulum_wrapper.h","r")
hEmptyOutputFile = open("./src/training/pendulum_wrapper_empty.h", "w")
hSolutionOutputFile = open("./src/training/pendulum_wrapper_solution.h", "w")

if(not cppInputFile or not cppEmptyOutputFile or not hInputFile or not hEmptyOutputFile):
	exit

## Filter cpp files
filterSolution(hInputFile, hEmptyOutputFile, hSolutionOutputFile)
filterSolution(cppInputFile, cppEmptyOutputFile, cppSolutionOutputFile)

# Close files
cppInputFile.close()
cppEmptyOutputFile.close()
cppSolutionOutputFile.close()
hInputFile.close()
hEmptyOutputFile.close()
hSolutionOutputFile.close()
