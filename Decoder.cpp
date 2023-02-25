#include "Decoder.h"

namespace symbol_namespace {
uint64_t GetInt(Symbol symbol) {
    uint64_t value = 0;
    for (size_t i = 0; i < BITS_IN_SYMBOL; ++i) {
        value |= (1ll << (BITS_IN_SYMBOL - i - 1)) * symbol[i];
    }
    return value;
}
}  // namespace symbol_namespace

Decoder::Decoder(std::ifstream &in) : in_(in) {
}

void Decoder::Decode() {
    while (true) {
        if (is_last_file_) {
            return;
        }
        DecodeFile();
        Clear();
    }
}

void Decoder::DecodeFile() {
    size_t alphabet_size = 0;
    try {
        alphabet_size = symbol_namespace::GetInt(in_.GetKBits(BITS_IN_SYMBOL));
        for (size_t i = 0; i < alphabet_size; ++i) {
            symbols_sorted_by_canonic_code_.emplace_back(in_.GetKBits(BITS_IN_SYMBOL));
        }
    } catch (std::exception &e) {
        throw std::runtime_error("Error: failed to read Alphabet Size.");
    }

    std::vector<size_t> cnt_code_lengths;
    try {
        GetCntCodeLengths(alphabet_size, cnt_code_lengths);
    } catch (std::exception &e) {
        throw std::runtime_error("Error: failed to get number of code lengths.");
    }

    std::vector<std::pair<Symbol, size_t>> code_lengths;
    GetCodeLengths(alphabet_size, cnt_code_lengths, code_lengths);

    std::unordered_map<Symbol, Code> codes;
    code_namespace::GetCanonicalCodes(code_lengths, codes, symbols_sorted_by_canonic_code_);

    for (auto &[symbol, code] : codes) {
        decodes_[code] = symbol;
    }

    std::string file_name = GetNameOfFile(FILENAME_END);
    std::ofstream out(file_name);

    try {
        DecodeContentOfFile(out);
    } catch (std::exception &e) {
        throw std::runtime_error("Error: failed to decode content of file.");
    }
}

void Decoder::GetCntCodeLengths(size_t alphabet_size, std::vector<size_t> &cnt_code_lengths) {
    size_t left_symbols = alphabet_size;
    while (left_symbols > 0) {
        size_t cnt = symbol_namespace::GetInt(in_.GetKBits(BITS_IN_SYMBOL));
        cnt_code_lengths.emplace_back(cnt);
        left_symbols -= cnt;
    }
}

void Decoder::GetCodeLengths(size_t alphabet_size, std::vector<size_t> &cnt_code_lengths,
                             std::vector<std::pair<Symbol, size_t>> &code_lengths) {
    size_t cur_length = 1;
    std::reverse(cnt_code_lengths.begin(), cnt_code_lengths.end());
    for (size_t i = 0; i < alphabet_size; ++i) {
        while (cnt_code_lengths.back() == 0) {
            ++cur_length;
            cnt_code_lengths.pop_back();
        }
        --cnt_code_lengths.back();
        code_lengths.emplace_back(std::make_pair(symbols_sorted_by_canonic_code_[i], cur_length));
    }
}

std::string Decoder::GetNameOfFile(const Symbol &splitter) {
    Code current;
    std::string str;
    while (true) {
        current.emplace_back(in_.GetBit());
        if (decodes_.contains(current)) {
            if (decodes_[current] == splitter) {
                return str;
            }
            str += GetCharBySymbol(decodes_[current]);
            current.clear();
        }
    }
}

void Decoder::DecodeContentOfFile(std::ofstream &out) {
    Code current;
    while (current != ARCHIVE_END && current != ONE_MORE_FILE) {
        current.emplace_back(in_.GetBit());
        if (decodes_.contains(current)) {
            if (decodes_[current] == ARCHIVE_END || decodes_[current] == ONE_MORE_FILE) {
                is_last_file_ = (decodes_[current] == ARCHIVE_END);
                return;
            }
            out.put(GetCharBySymbol(decodes_[current]));
            current.clear();
        }
    }
}

void Decoder::Clear() {
    symbols_sorted_by_canonic_code_.clear();
    decodes_.clear();
}

Decoder::~Decoder() {
}
