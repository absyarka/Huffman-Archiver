#pragma once

#include "Libraries.h"

#include "Reader.h"

class Decoder {
public:
    explicit Decoder(std::ifstream &in);

    void Decode();

    ~Decoder();

private:
    void GetCntCodeLengths(size_t alphabet_size, std::vector<size_t> &cnt_code_lengths);

    void GetCodeLengths(size_t alphabet_size, std::vector<size_t> &cnt_code_lengths,
                        std::vector<std::pair<Symbol, size_t>> &code_lengths);

    std::string GetNameOfFile(const Symbol &splitter = FILENAME_END);

    void DecodeFile();

    void DecodeContentOfFile(std::ofstream &out);

    void Clear();

    Reader in_;
    bool is_last_file_ = false;
    std::vector<Symbol> symbols_sorted_by_canonic_code_;
    std::unordered_map<Code, Symbol> decodes_;
};
