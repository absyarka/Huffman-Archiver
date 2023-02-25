#include "Encoder.h"

#include "PriorityQueue.h"

void Encoder::AddFile(const std::string &file_name) {
    if (!is_first_file_) {
        out_.Write(codes_[ONE_MORE_FILE]);
    }
    Clear();
    std::unordered_map<Symbol, size_t> freq;
    for (auto &c : file_name) {
        ++freq[GetSymbol(static_cast<unsigned char>(c))];
    }
    std::ifstream in(file_name, std::ios::binary);
    while (in.peek() != EOF) {
        char c = static_cast<char>(in.get());
        ++freq[GetSymbol(static_cast<unsigned char>(c))];
    }
    in.close();
    ++freq[FILENAME_END];
    ++freq[ONE_MORE_FILE];
    ++freq[ARCHIVE_END];
    MakeCanonical(freq);
    Encode(file_name);
}

void Encoder::MakeCanonical(std::unordered_map<Symbol, size_t> &freq) {
    auto cmp = [](const std::pair<size_t, HuffmanTree *> &lhs, const std::pair<size_t, HuffmanTree *> &rhs) {
        return std::tie(lhs.first, lhs.second->ReceiveSymbol()) > std::tie(rhs.first, rhs.second->ReceiveSymbol());
    };
    PriorityQueue<std::pair<size_t, HuffmanTree *>, decltype(cmp)> priority_queue;
    for (auto &[symbol, cnt] : freq) {
        HuffmanTree *tmp = new HuffmanTree(symbol);
        priority_queue.Push({cnt, tmp});
    }
    while (priority_queue.Size() > 1) {
        auto lhs = priority_queue.Extract();
        auto rhs = priority_queue.Extract();
        priority_queue.Push({lhs.first + rhs.first, HuffmanTree::Merge(lhs.second, rhs.second)});
    }
    auto code_lengths = priority_queue.Top().second->CodeLengths();
    delete priority_queue.Top().second;
    std::sort(code_lengths.begin(), code_lengths.end(), [](const auto &lhs, const auto &rhs) -> bool {
        return std::tie(lhs.second, lhs.first) < std::tie(rhs.second, rhs.first);
    });
    code_namespace::GetCanonicalCodes(code_lengths, codes_, symbols_, true);
}

void Encoder::Encode(const std::string &file_name) {
    out_.Write(symbols_.size(), BITS_IN_SYMBOL);
    for (const auto &symbol : symbols_) {
        out_.Write(symbol);
    }
    std::vector<size_t> cnt_code_lengths;
    for (const auto &[symbol, code] : codes_) {
        while (cnt_code_lengths.size() < code.size()) {
            cnt_code_lengths.emplace_back(0);
        }
        ++cnt_code_lengths[code.size() - 1];
    }
    for (auto cnt : cnt_code_lengths) {
        out_.Write(cnt, BITS_IN_SYMBOL);
    }
    for (char c : file_name) {
        out_.Write(codes_[GetSymbol(static_cast<unsigned char>(c))]);
    }
    out_.Write(codes_[FILENAME_END]);
    std::ifstream in(file_name, std::ios::binary);
    while (in.peek() != EOF) {
        char c = static_cast<char>(in.get());
        out_.Write(codes_[GetSymbol(static_cast<unsigned char>(c))]);
    }
    in.close();
}

Encoder::Encoder(std::ofstream &out) : out_(out) {
}

void Encoder::ArchiveEnd() {
    out_.Write(codes_[ARCHIVE_END]);
    out_.Close();
    Clear();
}

void Encoder::Clear() {
    symbols_.clear();
    codes_.clear();
    is_first_file_ = false;
}

void Encoder::PrintCodes() const {
    for (auto &el : codes_) {
        for (const auto &x : el.first) {
            std::cout << x;
        }
        std::cout << std::endl;
        for (const auto &x : el.second) {
            std::cout << x << ' ';
        }
        std::cout << std::endl << std::endl;
    }
    std::cout << std::endl;
}

Encoder::~Encoder() {
}
