#include "Libraries.h"

namespace code_namespace {
void CodePlusOne(Code &code) {
    size_t cnt = 0;
    while (!code.empty() && code.back()) {
        ++cnt;
        code.pop_back();
    }
    if (code.empty()) {
        code.emplace_back(true);
    } else {
        code.back() = true;
    }
    while (cnt > 0) {
        --cnt;
        code.push_back(false);
    }
}

void ShiftCode(Code &code, size_t shift) {
    while (shift > 0) {
        --shift;
        code.push_back(false);
    }
}

void GetCanonicalCodes(const std::vector<std::pair<Symbol, size_t>> &code_lengths,
                       std::unordered_map<Symbol, Code> &codes, std::vector<Symbol> &symbols, bool add_symbols) {
    Code code;
    for (size_t i = 0; i < code_lengths.size(); ++i) {
        size_t len = code_lengths[i].second;
        if (i == 0) {
            code_namespace::ShiftCode(code, len);
        }
        Symbol symbol = code_lengths[i].first;
        if (add_symbols) {
            symbols.emplace_back(symbol);
        }
        for (size_t bit = 0; bit < len; ++bit) {
            codes[symbol].emplace_back(code[bit]);
        }
        if (i + 1 < code_lengths.size()) {
            size_t new_length = code_lengths[i + 1].second;
            code_namespace::CodePlusOne(code);
            code_namespace::ShiftCode(code, new_length - len);
        }
    }
}
}  // namespace code_namespace

char GetCharBySymbol(const Symbol &symbol) {
    unsigned char c = 0;
    for (size_t i = 1; i < BITS_IN_SYMBOL; ++i) {
        c |= (1ll << (BITS_IN_SYMBOL - i - 1)) * symbol[i];
    }
    return static_cast<char>(c);
}