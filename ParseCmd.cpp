#include "ParseCmd.h"

ParseCmd::ParseCmd() : result_(Result::Help) {
    PrintHelp();
}

ParseCmd::ParseCmd(int argc, char **argv) {
    if (argc <= 1) {
        result_ = Result::Error;
        error_message_ = "Error: few arguments.";
        return;
    }
    if (argc == 2) {
        if (static_cast<std::string>(*(argv + 1)) == "-h") {
            result_ = Result::Help;
        } else {
            result_ = Result::Error;
            error_message_ = "Error: incorrect arguments.";
        }
        return;
    }
    if (static_cast<std::string>(*(argv + 1)) == "-d") {
        if (argc != 3) {
            result_ = Result::Error;
            error_message_ = "Error: incorrect arguments.";
            return;
        }
        result_ = Result::Decode;
        archive_name_ = *(argv + 2);
        if (!FileInDir(archive_name_)) {
            result_ = Result::Error;
            error_message_ = "Error: no such file in directory";
        }
        return;
    }
    if (static_cast<std::string>(*(argv + 1)) == "-c") {
        archive_name_ = *(argv + 2);
        for (int index = 3; index < argc; ++index) {
            file_names_.push_back(*(argv + index));
        }
        result_ = Result::Encode;
        AllFilesInDir();
        return;
    }
    result_ = Result::Error;
    error_message_ = "Error: incorrect arguments.";
}

void ParseCmd::PrintHelp(bool full_stats) const {
    using std::endl;
    if (!full_stats) {
        std::cout << "archiver -h" << endl << "\t to see archiver functions" << endl;
        return;
    }
    std::cout << "archiver -c archive_name file1 [file2 ...]" << endl
              << "\tto generate an archive with name *archive_name* in current directory" << endl
              << endl
              << "archiver -d archive_name" << endl
              << "\tto extract files from archive named *archive_name* in current directory" << endl
              << endl
              << "archiver -h" << endl
              << "\t to see archiver functions" << endl;
}

bool ParseCmd::FileInDir(const std::string &file) {
    if (!std::filesystem::exists(file)) {
        result_ = Result::Error;
        error_message_ = "Error: " + file + " not in directory";
        return false;
    }
    return true;
}

void ParseCmd::AllFilesInDir() {
    for (const auto &file : file_names_) {
        if (!FileInDir(file)) {
            break;
        }
    }
}

ParseCmd::~ParseCmd() {
}
