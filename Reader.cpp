#include "Reader.h"

Reader::Reader(std::ifstream &in) : in_(in) {
}

bool Reader::GetBit() {
    if (buffer_.empty()) {
        if (in_.peek() == EOF) {
            throw std::runtime_error("Error: no chars left.");
        }
        unsigned char c = static_cast<unsigned char>(in_.get());
        for (size_t i = 0; i < BITS_IN_CHAR; ++i) {
            buffer_.emplace_back(c & (1 << i));
        }
    }
    bool bit = buffer_.back();
    buffer_.pop_back();
    return bit;
}

std::vector<bool> Reader::GetKBits(size_t k) {
    std::vector<bool> bits;
    while (bits.size() < k) {
        bits.emplace_back(GetBit());
    }
    return bits;
}

void Reader::CloseStream() {
    in_.close();
}

Reader::~Reader() {
    CloseStream();
}
