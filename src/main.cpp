#include <cstdlib>
#include <iostream>
#include <regex>
#include <fstream>

// Function to extract the executable name from CMakeLists.txt
std::string getExecutableName(const std::string& cmakeListsPath) {
    std::ifstream cmakeListsFile(cmakeListsPath);

    if (!cmakeListsFile.is_open()) {
        std::cerr << "Failed to open CMakeLists.txt file." << std::endl;
        return "";
    }

    std::regex targetNameRegex(R"(add_executable\((\S+))");
    std::smatch match;

    for (std::string line; std::getline(cmakeListsFile, line);) {
        if (std::regex_search(line, match, targetNameRegex)) {
            return match[1].str();
        }
    }

    std::cerr << "Failed to extract executable name from CMakeLists.txt." << std::endl;
    return "";
}

int configureProject(const std::string& projectDir, const std::string& buildDir) {
    // Create the build directory if it doesn't exist
    std::string mkdirCommand = "mkdir -p " + buildDir;
    int mkdirResult = std::system(mkdirCommand.c_str());

    if (mkdirResult != 0) {
        std::cerr << "Failed to create build directory." << std::endl;
        return mkdirResult;
    }

    // Clean the build directory
    std::string cleanCommand = "cmake --build " + buildDir + " --target clean";
    int cleanResult = std::system(cleanCommand.c_str());

    if (cleanResult != 0) {
        std::cerr << "Clean build directory failed." << std::endl;
        //return cleanResult;
    }

    // Run CMake to configure the project into the specified build directory
    std::string cmakeCommand = "cmake -S " + projectDir + " -B " + buildDir;
    int result = std::system(cmakeCommand.c_str());

    if (result == 0) {
        std::cout << "CMake configuration successful." << std::endl;
    } else {
        std::cerr << "CMake configuration failed." << std::endl;
    }

    // Build the project using make
    std::string makeCommand = "make -C " + buildDir;
    int makeResult = std::system(makeCommand.c_str());

    if (makeResult != 0) {
        std::cerr << "Build failed." << std::endl;
        return makeResult;
    }


    return result;
}


int runStaticAnalyzer(const std::string buildDir, const std::string executableName)
{
    // Run Clang Static Analyzer on the executable
    std::string scanBuildCommand = "scan-build -o "  + buildDir + "make -C " + buildDir;;
    int scanBuildResult = std::system(scanBuildCommand.c_str());

    if (scanBuildResult == 0) {
        std::cout << "Clang Static Analyzer completed successfully." << std::endl;
    } else {
        std::cerr << "Clang Static Analyzer failed." << std::endl;
    }

    return scanBuildResult;
}



int main(int argc, const char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <project_directory>" 
         << std::endl;
        return 1;
    }

    std::string projectDir(argv[1]);
    std::string buildDir(projectDir+"/build");

    // Step 1: Configure the project
    if (configureProject(projectDir, buildDir) != 0) {
        return 1;
    }

    // Obtain the executable name dynamically from CMakeLists.txt
    std::string cmakeListsPath = projectDir + "/CMakeLists.txt";
    std::string executableName = getExecutableName(cmakeListsPath);
    
    // Step 2: Generate the compilation database
    if (runStaticAnalyzer(buildDir, executableName) != 0) {
        return 1;
    }


    return 0;
}

