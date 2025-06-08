#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>

#ifdef _WIN32
#include <windows.h> // For SetFileAttributes on Windows
#else
#include <sys/stat.h> // For chmod on Unix-like systems
#endif

// Function to create a hidden .comment file
void createHiddenCommentFile(const std::string& path, const std::string& comment) {
    std::string commentFilePath = path + "/.comment";

    std::ofstream commentFile(commentFilePath);
    if (commentFile.is_open()) {
        commentFile << comment;
        commentFile.close();

        // Set file as hidden
        #ifdef _WIN32
        SetFileAttributes(commentFilePath.c_str(), FILE_ATTRIBUTE_HIDDEN);
        #else
        // On Unix-like systems, prefix with dot makes it hidden
        // No additional action needed beyond creating the file
        #endif

        std::cout << "Comment created successfully at: " << commentFilePath << std::endl;
    } else {
        std::cerr << "Error: Unable to create comment file at: " << commentFilePath << std::endl;
    }
}

// Function to append to the .comment file
void appendCommentFile(const std::string& path, const std::string& comment) {
    std::string commentFilePath = path + "/.comment";

    std::ofstream commentFile(commentFilePath, std::ios::app);
    if (commentFile.is_open()) {
        commentFile << std::endl;
        commentFile << comment;
        commentFile.close();

        #ifdef _WIN32
        SetFileAttributes(commentFilePath.c_str(), FILE_ATTRIBUTE_HIDDEN);
        #endif

        std::cout << "Comment appended successfully to: " << commentFilePath << std::endl;
    } else {
        std::cerr << "Error: Unable to append to comment file at: " << commentFilePath << std::endl;
    }
}

// Function to remove the .comment file
void removeCommentFile(const std::string& path) {
    std::string commentFilePath = path + "/.comment";

    if (std::remove(commentFilePath.c_str()) == 0) {
        std::cout << "Comment removed successfully from: " << path << std::endl;
    } else {
        std::cerr << "Error: Unable to remove comment file or file doesn't exist at: " << commentFilePath << std::endl;
    }
}

// Function to view the content of the .comment file
void viewCommentFile(const std::string& path) {
    std::string commentFilePath = path + "/.comment";

    std::ifstream commentFile(commentFilePath);
    if (commentFile.is_open()) {
        std::cout << "Comment content:" << std::endl;
        std::cout << "----------------" << std::endl;
        std::string line;
        while (std::getline(commentFile, line)) {
            std::cout << line << std::endl;
        }
        commentFile.close();
    } else {
        std::cerr << "Error: No comment file found at: " << commentFilePath << std::endl;
    }
}

// Function to read comment from a text file
std::string readCommentFromFile(const std::string& filePath) {
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open text file: " << filePath << std::endl;
        return "";
    }

    std::string comment((std::istreambuf_iterator<char>(inputFile)),
                        std::istreambuf_iterator<char>());
    inputFile.close();

    return comment;
}

// Function to read comment from stdin
std::string readCommentFromStdin() {
    std::cout << "Enter your comment (press Ctrl+D on empty line to finish on Linux/Mac, Ctrl+Z on Windows):" << std::endl;

    std::string comment;
    std::string line;

    while (std::getline(std::cin, line)) {
        comment += line + "\n";
    }

    // Clear the error state caused by Ctrl+D/Ctrl+Z
    std::cin.clear();

    return comment;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " [--comment|--append-comment|--remove-comment|--view-comment] <path> [--from-command-line \"text\"|--from-text-file /path/to/file.txt|--from-stdin]" << std::endl;
        return 1;
    }

    std::string action = argv[1];
    std::string path = argv[2];

    if (action == "--comment" || action == "--append-comment") {
        std::string comment;

        if (argc >= 5 && std::string(argv[3]) == "--from-command-line") {
            comment = argv[4];
        }
        else if (argc >= 5 && std::string(argv[3]) == "--from-text-file") {
            comment = readCommentFromFile(argv[4]);
        }
        else if (argc >= 4 && std::string(argv[3]) == "--from-stdin") {
            comment = readCommentFromStdin();
        }
        else {
            std::cerr << "Error: Missing comment source specification after " << action << std::endl;
            std::cerr << "Use one of: --from-command-line \"text\", --from-text-file /path/to/file.txt, or --from-stdin" << std::endl;
            return 1;
        }

        if (!comment.empty()) {
            if (action == "--comment") {
                createHiddenCommentFile(path, comment);
            } else {
                appendCommentFile(path, comment);
            }
        } else {
            std::cerr << "Error: No comment provided or could not read comment" << std::endl;
            return 1;
        }
    }
    else if (action == "--remove-comment") {
        removeCommentFile(path);
    }
    else if (action == "--view-comment") {
        viewCommentFile(path);
    }
    else {
        std::cerr << "Error: Unknown action. Use --comment, --append-comment, --remove-comment, or --view-comment" << std::endl;
        return 1;
    }

    return 0;
}
