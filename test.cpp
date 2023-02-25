#include <catch.hpp>

#include "Libraries.h"

#include "Encoder.h"
#include "PriorityQueue.h"

#include <random>
#include <set>
#include <time.h>

std::mt19937_64 rand_int(time(0));

TEST_CASE("GettingCodesByFrequencies") {
    {
        std::cout << "Case 1:" << std::endl;
        std::unordered_map<Symbol, size_t> freq = {{GetSymbol(static_cast<unsigned char>('a')), 45},  //
                                                   {ARCHIVE_END, 1},                                  //
                                                   {ONE_MORE_FILE, 1},                                //
                                                   {FILENAME_END, 1}};                                //
        std::ofstream out("output.txt");
        Encoder encoder(out);
        encoder.MakeCanonical(freq);
        encoder.PrintCodes();
        out.close();
        REQUIRE(true);
    }
    {
        std::cout << "Case 2:" << std::endl;
        std::unordered_map<Symbol, size_t> freq = {{GetSymbol(static_cast<unsigned char>('a')), 3},   //
                                                   {GetSymbol(static_cast<unsigned char>('b')), 2},   //
                                                   {GetSymbol(static_cast<unsigned char>('c')), 2},   //
                                                   {GetSymbol(static_cast<unsigned char>('d')), 1}};  //
        std::ofstream out("output.txt");
        Encoder encoder(out);
        encoder.MakeCanonical(freq);
        encoder.PrintCodes();
        out.close();
        REQUIRE(true);
    }
    {
        std::cout << "Case 3:" << std::endl;
        std::unordered_map<Symbol, size_t> freq = {{GetSymbol(static_cast<unsigned char>('a')), 5},   //
                                                   {GetSymbol(static_cast<unsigned char>('b')), 2},   //
                                                   {GetSymbol(static_cast<unsigned char>('c')), 2},   //
                                                   {GetSymbol(static_cast<unsigned char>('d')), 1}};  //
        std::ofstream out("output.txt");
        Encoder encoder(out);
        encoder.MakeCanonical(freq);
        encoder.PrintCodes();
        out.close();
        REQUIRE(true);
    }
    {
        std::cout << "Case 4:" << std::endl;
        std::unordered_map<Symbol, size_t> freq = {{GetSymbol(static_cast<unsigned char>('a')), 5},   //
                                                   {GetSymbol(static_cast<unsigned char>('b')), 3},   //
                                                   {GetSymbol(static_cast<unsigned char>('c')), 4},   //
                                                   {GetSymbol(static_cast<unsigned char>('d')), 4}};  //
        std::ofstream out("output.txt");
        Encoder encoder(out);
        encoder.MakeCanonical(freq);
        encoder.PrintCodes();
        out.close();
        REQUIRE(true);
    }
    {
        std::cout << "Case 5:" << std::endl;
        std::unordered_map<Symbol, size_t> freq = {{GetSymbol(static_cast<unsigned char>('a')), 7},   //
                                                   {GetSymbol(static_cast<unsigned char>('b')), 4},   //
                                                   {GetSymbol(static_cast<unsigned char>('c')), 4},   //
                                                   {GetSymbol(static_cast<unsigned char>('d')), 3},   //
                                                   {GetSymbol(static_cast<unsigned char>('e')), 1},   //
                                                   {GetSymbol(static_cast<unsigned char>('f')), 1},   //
                                                   {GetSymbol(static_cast<unsigned char>('g')), 1}};  //
        std::ofstream out("output.txt");
        Encoder encoder(out);
        encoder.MakeCanonical(freq);
        encoder.PrintCodes();
        out.close();
        REQUIRE(true);
    }
}  //  using eyes to check if it works (drawing result tree on paper)

TEST_CASE("PriorityQueue") {
    {
        PriorityQueue<int> priority_queue;
        priority_queue.Push(1);
        priority_queue.Push(2);
        priority_queue.Push(4);
        priority_queue.Push(3);
        REQUIRE(priority_queue.Extract() == 4);
        REQUIRE(priority_queue.Extract() == 3);
        REQUIRE(priority_queue.Size() == 2);
        priority_queue.Push(5);
        REQUIRE(priority_queue.Top() == 5);
        REQUIRE(priority_queue.Extract() == 5);
        REQUIRE(priority_queue.Extract() == 2);
    }
    {
        const auto cmp = [](const std::set<int64_t>& lhs, const std::set<int64_t>& rhs) -> bool {
            if (lhs.empty()) {
                return !rhs.empty();
            }
            if (rhs.empty()) {
                return false;
            }
            return *(lhs.begin()) % 5 < *(rhs.begin()) % 5;
        };
        PriorityQueue<std::set<int64_t>, decltype(cmp)> priority_queue;
        std::multiset<std::set<int64_t>, decltype(cmp)> multiset;
        size_t n = abs(static_cast<int64_t>(rand_int()) % 1000);

        auto add_elem = [&]() -> void {
            std::set<int64_t> elems;
            size_t m = rand_int() % 9 + 1;
            for (size_t j = 0; j < m; ++j) {
                elems.insert(abs(static_cast<int64_t>(rand_int()) % 2000));
            }
            multiset.insert(elems);
            priority_queue.Push(elems);
        };
        auto check_with_remove = [&]() -> bool {
            const auto elems1 = *multiset.begin();
            multiset.erase(multiset.begin());
            const auto elems2 = priority_queue.Extract();
            return (*elems1.begin() % 5 == *elems2.begin() % 5);
        };
        auto check_without_remove = [&]() -> bool {
            const auto& elems1 = *multiset.begin();
            const auto& elems2 = priority_queue.Top();
            return (*elems1.begin() % 5 == *elems2.begin() % 5);
        };

        for (size_t i = 0; i < n; ++i) {
            add_elem();
        }

        for (size_t i = 0; i < n / 2; ++i) {
            REQUIRE(check_with_remove());
        }
        while (!multiset.empty()) {
            add_elem();
            REQUIRE(check_with_remove());
            REQUIRE(check_without_remove());
            REQUIRE(check_with_remove());
        }
    }
}