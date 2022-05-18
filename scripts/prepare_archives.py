# This script prepares zip archives for the tutorial
#
# Author: K. Desnos
# License: CeCILL-C

from fileinput import filename
import re
from zipfile import ZipFile
import os
from os.path import basename

# Add a file to the zip archive, with an additional parentName folder.
def zipFileAdd(zipObj, fileName, parentName = ""):
    zipObj.write(fileName, parentName + fileName)

# Zip the files from given directory that matches the filter, includes the subdirectories
# keeps the relative structure
# Adapted from https://thispointer.com/python-how-to-create-a-zip-archive-from-multiple-files-or-directory/
def zipFilesInDir(dirName, zipObj, regex, parentName="", withSubdirectories = True):
    if(withSubdirectories):
        # Iterate over all the files in directory
        for folderName, subfolders, filenames in os.walk(dirName):
            for filename in filenames:
                if (re.match(regex, filename)):
                    # create complete filepath of file in directory
                    filePath = os.path.join(folderName, filename)
                    # Add file to zip 
                    zipObj.write(filePath, parentName + filePath)
    else:
        # Iterate over all the files in directory
        for filename in os.listdir(dirName):
                # create complete filepath of file in directory
                filePath = os.path.join(dirName, filename)
                if (os.path.isfile(filePath) and re.match(regex, filename)):
                    # create complete filepath of file in directory
                    filePath = os.path.join(dirName, filename)
                    # Add file to zip 
                    zipObj.write(filePath, parentName + filePath)



# Create the tutorialTemplate archive
mainFolder = "tutorial-gegelati/"
tutorialTemplateArchive = ZipFile("./docs/data/gegelati-tutorial.zip", "w")
zipFileAdd(tutorialTemplateArchive,"bin/", mainFolder)
zipFileAdd(tutorialTemplateArchive,"dat/download_dat.sh", mainFolder)
zipFilesInDir("./",tutorialTemplateArchive, r'[^\.]+.*', mainFolder, False) # exclude .gitgnore
zipFilesInDir("./lib/",tutorialTemplateArchive, r'.*', mainFolder)
zipFilesInDir("src/",tutorialTemplateArchive, r'.*', mainFolder, False)
zipFilesInDir("src/manual/",tutorialTemplateArchive, r'.*', mainFolder)
zipFilesInDir("src/training",tutorialTemplateArchive, r'^(?!.*(pendulum_wrapper))', mainFolder, False) # all files except pendulum_wrapper
tutorialTemplateArchive.write("src/training/pendulum_wrapper_empty.cpp", mainFolder + "src/training/pendulum_wrapper.cpp" ) # overwrite empty_file
tutorialTemplateArchive.write("src/training/pendulum_wrapper_empty.h", mainFolder + "src/training/pendulum_wrapper.h") # overwrite empty_file
tutorialTemplateArchive.close()

# Create the pendulum_wrapper_solution archive
pendulumWrapperSolutionArchive = ZipFile("./docs/data/pendulum_wrapper_solution.zip", "w")
pendulumWrapperSolutionArchive.write("src/training/pendulum_wrapper_solution.cpp", "pendulum_wrapper.cpp") # overwrite empty_file
pendulumWrapperSolutionArchive.write("src/training/pendulum_wrapper_solution.h", "pendulum_wrapper.h") # overwrite empty_file
pendulumWrapperSolutionArchive.close()