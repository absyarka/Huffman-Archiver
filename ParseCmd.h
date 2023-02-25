#pragma once

#include "Libraries.h"

class ParseCmd {
public:
    enum class Result { Help, Decode, Encode, Error };

    ParseCmd();

    ParseCmd(int argc, char **argv);

    void PrintHelp(bool full_stats = true) const;

    Result result_;
    std::string archive_name_;
    std::vector<std::string> file_names_;
    std::string error_message_;

    ~ParseCmd();

private:
    bool FileInDir(const std::string &file);

    void AllFilesInDir();
};