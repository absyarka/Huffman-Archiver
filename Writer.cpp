#include "Writer.h"

Writer::Writer(std::ofstream &out) : out_(out) {
}

void Writer::Write(uint64_t val, size_t len) {
    std::vector<bool> bits;
    for (size_t i = 0; i < len; ++i) {
        bits.emplace_back(val & 1);
        val >>= 1;
    }
    std::reverse(bits.begin(), bits.end());
    Write(bits);
}

void Writer::Write(const std::vector<bool> &bits) {
    for (auto bit : bits) {
        buffer_.emplace_back(bit);
        if (buffer_.size() == BITS_IN_CHAR) {
            Write();
        }
    }
}

void Writer::Write() {
    unsigned char c = 0;
    for (size_t i = 0; i < BITS_IN_CHAR; ++i) {
        c |= (1ll << (BITS_IN_CHAR - i - 1)) * buffer_[i];
    }
    out_.put(static_cast<char>(c));
    buffer_.clear();
}

void Writer::Close() {
    if (!buffer_.empty()) {
        while (buffer_.size() < BITS_IN_CHAR) {
            buffer_.emplace_back(false);
        }
        Write();
    }
    out_.close();
}

Writer::~Writer() {
    Close();
}
