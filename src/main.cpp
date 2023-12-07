#include <iostream>
#include <string.h>
#include <filesystem>


namespace fs = std::filesystem;

std::string getCMakeProjectDirectory(int argc, char* argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " -f <input_string>" << std::endl;
        exit(1); // Exit with an error code
    }

    // Check if the option is "-f"
    std::string option = argv[1];
    if (option != "-f") {
        std::cerr << "Invalid option. Use -f to specify the input string." << std::endl;
        exit(1); // Exit with an error code
    }

    // Return the input string
    return argv[2];
}


std::string findCMakeLists(const std::string& projectDirectory)
{
    for (const auto& entry : fs::recursive_directory_iterator(projectDirectory)) {
        if (entry.is_regular_file() && entry.path().filename() == "CMakeLists.txt") {
            return entry.path().string();
        }
    }

    return "";
}

void processCMakeProject(const std::string& projectDirectory)
{

    std::string cmakeListsPath = findCMakeLists(projectDirectory);

    if (cmakeListsPath.empty())
    {
        std::cerr << "Error CMakeLists.txt not found\n";
        exit(1);
    }

    
}

int main(int argc, char* argv[])
{
    std::string dir = getCMakeProjectDirectory(argc, argv);

    processCMakeProject(dir);


    return 0;
}
