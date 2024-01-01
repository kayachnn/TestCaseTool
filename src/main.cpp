#include <cstdlib>
#include <iostream>

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

    return result;
}



int generateCompilationDatabase(const std::string& projectDir) {
    std::string cmakeCommand = "cmake --build " + projectDir + " --target clean";
    int result = std::system(cmakeCommand.c_str());

    if (result == 0) {
        std::cout << "Clean build directory successful." << std::endl;
    } else {
        std::cerr << "Clean build directory failed." << std::endl;
        return result;
    }

    cmakeCommand = "cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON " + projectDir;
    result = std::system(cmakeCommand.c_str());

    if (result == 0) {
        std::cout << "Compilation database generation successful." << std::endl;
    } else {
        std::cerr << "Compilation database generation failed." << std::endl;
    }

    return result;
}

int runClangTool(const std::string& buildDir) {
    // Command to build the Clang tool
    std::string buildCommand = "clang++ -std=c++14 ClangAnalyseTool.cpp -o ClangAnalyseTool -lclangTooling -lclangASTMatchers -lclangAST -lclangBasic -lclangFrontend -lclangSerialization -lLLVMSupport -lpthread";
    
    int buildResult = std::system(buildCommand.c_str());

    if (buildResult != 0) {
        std::cerr << "Clang tool build failed." << std::endl;
        return buildResult;
    }

    // Command to run the Clang tool
    std::string runCommand = "./YourClangTool -p " + buildDir;
    int runResult = std::system(runCommand.c_str());

    if (runResult != 0) {
        std::cerr << "Clang tool run failed." << std::endl;
        return runResult;
    }

    return 0;
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

    // Step 2: Generate the compilation database
    if (generateCompilationDatabase(projectDir) != 0) {
        return 1;
    }

    // Step 3: Run the Clang tool on the target project
    if (runClangTool(projectDir) != 0) {
        return 1;
    }

    return 0;
}

