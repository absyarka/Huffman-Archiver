#pragma once

#include "Libraries.h"

#include "HuffmanTree.h"
#include "Writer.h"

class Encoder {
public:
    explicit Encoder(std::ofstream &out);

    void AddFile(const std::string &file_name);

    void Encode(const std::string &file_name);

    void ArchiveEnd();

    void MakeCanonical(std::unordered_map<Symbol, size_t> &freq);

    void PrintCodes() const;

    ~Encoder();

private:
    void Clear();

    Writer out_;
    bool is_first_file_ = true;
    std::vector<Symbol> symbols_;
    std::unordered_map<Symbol, Code> codes_;
};