#include "HuffmanTree.h"

HuffmanTree::HuffmanTree() {
}

HuffmanTree::HuffmanTree(Symbol symbol) : symbol(symbol) {
}

bool HuffmanTree::IsLeaf() const {
    return (left == nullptr && right == nullptr);
}

HuffmanTree *HuffmanTree::Merge(HuffmanTree *lhs, HuffmanTree *rhs) {
    HuffmanTree *new_tree = new HuffmanTree();
    new_tree->left = lhs;
    new_tree->right = rhs;
    if (!new_tree->IsLeaf()) {
        new_tree->symbol = std::min(lhs->symbol, rhs->symbol);
    }
    return new_tree;
}

const Symbol HuffmanTree::ReceiveSymbol() const {
    return symbol;
}

std::vector<std::pair<Symbol, size_t>> HuffmanTree::CodeLengths() const {
    std::vector<std::pair<Symbol, size_t>> code_lengths;
    DFS(this, code_lengths);
    return code_lengths;
}

void HuffmanTree::DFS(const HuffmanTree *node, std::vector<std::pair<Symbol, size_t>> &code_lengths,
                      size_t depth) const {
    if (node == nullptr) {
        return;
    }
    DFS(node->left, code_lengths, depth + 1);
    DFS(node->right, code_lengths, depth + 1);
    if (node->IsLeaf()) {
        code_lengths.emplace_back(node->symbol, depth);
    }
}

HuffmanTree::~HuffmanTree() {
    delete left;
    delete right;
}
