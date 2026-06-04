# This script prepares zip archives for the tutorial
#
# Author: K. Desnos
# License: CeCILL-C

from fileinput import filename
import re
from zipfile import ZipFile
import os
from os.path import basename
import shutil

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

def replace_file_in_zip(zip_path, file_to_add, arcname):
    """
    Replace a file in a zip archive by first removing the existing file (if present),
    then adding the new file.
    """
    import tempfile

    # Create a temporary zip file
    tmpfd, tmpname = tempfile.mkstemp(suffix='.zip')
    os.close(tmpfd)
    with ZipFile(zip_path, 'r') as zin, ZipFile(tmpname, 'w') as zout:
        for item in zin.infolist():
            if item.filename != arcname:
                zout.writestr(item, zin.read(item.filename))
        # Now add the new file
        zout.write(file_to_add, arcname)
    # Replace the original zip with the modified one
    shutil.move(tmpname, zip_path)

# Create the tutorialTemplate archive
mainFolder = "gegelati-tutorial/"
tutorialTemplateArchive = ZipFile("./docs/data/gegelati-tutorial.zip", "w")
zipFileAdd(tutorialTemplateArchive,"bin/", mainFolder)
zipFilesInDir("./",tutorialTemplateArchive, r'^(?!.*(CMakeLists|params))[^\.]+.*', mainFolder, False) # exclude .gitgnore and CMakeLists files
zipFilesInDir("./dat/",tutorialTemplateArchive, r'.*', mainFolder)
zipFilesInDir("./lib/",tutorialTemplateArchive, r'.*', mainFolder)
zipFilesInDir("src/",tutorialTemplateArchive, r'.*', mainFolder, False)
zipFilesInDir("src/manual/",tutorialTemplateArchive, r'.*', mainFolder)
zipFilesInDir("src/training",tutorialTemplateArchive, r'^(?!.*(pendulum_wrapper|main-training))', mainFolder, False) # all files except pendulum_wrapper or main-training
tutorialTemplateArchive.write("src/training/pendulum_wrapper_empty.cpp", mainFolder + "src/training/pendulum_wrapper.cpp" ) # overwrite empty_file
tutorialTemplateArchive.write("src/training/pendulum_wrapper_empty.h", mainFolder + "src/training/pendulum_wrapper.h") # overwrite empty_file
tutorialTemplateArchive.write("src/training/main-training_empty.cpp", mainFolder + "src/training/main-training.cpp") # overwrite empty_file
tutorialTemplateArchive.write("CMakeLists_empty.txt", mainFolder + "CMakeLists.txt") # overwrite empty_file
tutorialTemplateArchive.write("params_empty.json", mainFolder + "params.json") # overwrite empty_file
tutorialTemplateArchive.close()

# Create the pendulum_wrapper_solution archive
pendulumWrapperSolutionArchive = ZipFile("./docs/data/pendulum_wrapper_solution.zip", "w")
pendulumWrapperSolutionArchive.write("src/training/pendulum_wrapper_solution.cpp", "pendulum_wrapper.cpp") # overwrite empty_file
pendulumWrapperSolutionArchive.write("src/training/pendulum_wrapper_solution.h", "pendulum_wrapper.h") # overwrite empty_file
pendulumWrapperSolutionArchive.close()


# Create the gegelati-tutorial-solution archive by copying the template and patching needed files
mainFolder = "gegelati-tutorial/"
shutil.copy2("./docs/data/gegelati-tutorial.zip", "./docs/data/gegelati-tutorial-solution.zip")
replace_file_in_zip("./docs/data/gegelati-tutorial-solution.zip", "src/training/pendulum_wrapper_solution.cpp", mainFolder + "src/training/pendulum_wrapper.cpp")
replace_file_in_zip("./docs/data/gegelati-tutorial-solution.zip", "src/training/pendulum_wrapper_solution.h", mainFolder + "src/training/pendulum_wrapper.h")

# Create the gegelati-tutorial-strengthening-solution archive by copying the solution archive
mainFolder = "gegelati-tutorial/"
shutil.copy2("./docs/data/gegelati-tutorial-solution.zip", "./docs/data/gegelati-tutorial-strengthening-solution.zip")
replace_file_in_zip("./docs/data/gegelati-tutorial-strengthening-solution.zip", "src/training/pendulum_wrapper_strengthening.cpp", mainFolder + "src/training/pendulum_wrapper.cpp")
replace_file_in_zip("./docs/data/gegelati-tutorial-strengthening-solution.zip", "src/training/pendulum_wrapper_strengthening.h", mainFolder + "src/training/pendulum_wrapper.h")
replace_file_in_zip("./docs/data/gegelati-tutorial-strengthening-solution.zip", "params_strengthening.json", mainFolder + "params.json")

# Create the gegelati-tutorial-parallel-solution archive by copying the solution archive
mainFolder = "gegelati-tutorial/"
shutil.copy2("./docs/data/gegelati-tutorial-strengthening-solution.zip", "./docs/data/gegelati-tutorial-parallel-solution.zip")
replace_file_in_zip("./docs/data/gegelati-tutorial-parallel-solution.zip", "src/training/pendulum_wrapper_parallel.cpp", mainFolder + "src/training/pendulum_wrapper.cpp")
replace_file_in_zip("./docs/data/gegelati-tutorial-parallel-solution.zip", "src/training/pendulum_wrapper_parallel.h", mainFolder + "src/training/pendulum_wrapper.h")
replace_file_in_zip("./docs/data/gegelati-tutorial-parallel-solution.zip", "src/training/main-training_parallel.cpp", mainFolder + "src/training/main-training.cpp")

# Make the main-inference.cpp file available for download
shutil.copy2("./src/inference/main-inference.cpp", "./docs/data/")

