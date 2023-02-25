#pragma once

#include "Libraries.h"

class Writer {
public:
    explicit Writer(std::ofstream &out);

    void Write(uint64_t val, size_t len);

    void Write(const std::vector<bool> &bits);

    ~Writer();

    void Close();

private:
    void Write();

    std::ofstream &out_;
    std::vector<bool> buffer_;
};
