#pragma once

#include <bitset>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

static const size_t BITS_IN_CHAR = 8;
static const size_t BITS_IN_SYMBOL = 9;
using Symbol = std::vector<bool>;
using Code = std::vector<bool>;

static Symbol GetSymbol(uint64_t value) {
    Symbol symbol;
    for (size_t i = 0; i < BITS_IN_SYMBOL; ++i) {
        symbol.emplace_back((value & (1ll << (BITS_IN_SYMBOL - i - 1))) > 0);
    }
    return symbol;
}

char GetCharBySymbol(const Symbol &symbol);

static const Symbol FILENAME_END = GetSymbol(256);
static const Symbol ONE_MORE_FILE = GetSymbol(257);
static const Symbol ARCHIVE_END = GetSymbol(258);
static const Symbol BAD_SYMBOL = GetSymbol(0);

namespace code_namespace {
void CodePlusOne(Code &code);

void ShiftCode(Code &code, size_t shift);

void GetCanonicalCodes(const std::vector<std::pair<Symbol, size_t>> &code_lengths,
                       std::unordered_map<Symbol, Code> &codes, std::vector<Symbol> &symbols, bool add_symbols = false);
}  // namespace code_namespace