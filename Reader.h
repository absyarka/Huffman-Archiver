#pragma once

#include "Libraries.h"

class Reader {
public:
    explicit Reader(std::ifstream &in);

    bool GetBit();

    std::vector<bool> GetKBits(size_t k);

    ~Reader();

private:
    std::ifstream &in_;
    std::vector<bool> buffer_;

    void CloseStream();
};
