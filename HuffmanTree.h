#pragma once

#include "Libraries.h"

struct HuffmanTree {
    HuffmanTree();

    explicit HuffmanTree(Symbol symbol);

    bool IsLeaf() const;

    static HuffmanTree *Merge(HuffmanTree *lhs, HuffmanTree *rhs);

    const Symbol ReceiveSymbol() const;

    std::vector<std::pair<Symbol, size_t>> CodeLengths() const;

    void DFS(const HuffmanTree *node, std::vector<std::pair<Symbol, size_t>> &code_lengths, size_t depth = 0) const;

    ~HuffmanTree();

    Symbol symbol = BAD_SYMBOL;
    HuffmanTree *left = nullptr;
    HuffmanTree *right = nullptr;
};