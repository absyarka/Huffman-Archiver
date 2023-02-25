#pragma once

#include <vector>

template <typename T, class Compare = std::greater<T>>
class PriorityQueue {
public:
    PriorityQueue();
    PriorityQueue(const PriorityQueue<T, Compare>& other);
    PriorityQueue(PriorityQueue<T, Compare>&& other);
    PriorityQueue<T, Compare>& operator=(const PriorityQueue<T, Compare>& other);
    PriorityQueue<T, Compare>& operator=(PriorityQueue<T, Compare>&& other);
    void Push(const T& el);
    const T& Top() const;
    T Top();
    T Extract();
    size_t Size() const;
    bool Empty() const;

private:
    void MoveDown(size_t index);

    std::vector<T> arr_;
};

template <typename T, class Compare>
PriorityQueue<T, Compare>::PriorityQueue() {
}

template <typename T, class Compare>
void PriorityQueue<T, Compare>::Push(const T& el) {
    size_t id = arr_.size();
    arr_.template emplace_back(el);
    auto GetParentId = [](size_t index) -> size_t {  //  NOLINT
        if (index == 0) {
            throw std::out_of_range("Error: no parent node at PriorityQueue.");
        }
        return (index - 1) / 2;
    };
    while (id != 0 && Compare()(arr_[id], arr_[GetParentId(id)])) {
        std::swap(arr_[id], arr_[GetParentId(id)]);
        id = GetParentId(id);
    }
}

template <typename T, class Compare>
const T& PriorityQueue<T, Compare>::Top() const {
    if (arr_.size() == 0) {
        throw std::out_of_range("Error: call Top() of an empty PriorityQueue is forbidden.");
    }
    return arr_.at(0);
}

template <typename T, class Compare>
T PriorityQueue<T, Compare>::Top() {
    if (arr_.size() == 0) {
        throw std::out_of_range("Error: call Top() of an empty PriorityQueue is forbidden.");
    }
    return arr_[0];
}

template <typename T, class Compare>
T PriorityQueue<T, Compare>::Extract() {
    const T& top_value = Top();
    std::swap(arr_[0], arr_.back());
    arr_.pop_back();
    MoveDown(0);
    return top_value;
}

template <typename T, class Compare>
void PriorityQueue<T, Compare>::MoveDown(size_t index) {
    std::vector<size_t> candidates;
    if (2 * index + 1 < arr_.size() && !Compare()(arr_[index], arr_[2 * index + 1])) {
        candidates.emplace_back(2 * index + 1);
    }
    if (2 * index + 2 < arr_.size() && !Compare()(arr_[index], arr_[2 * index + 2])) {
        candidates.emplace_back(2 * index + 2);
    }
    if (candidates.empty()) {
        return;
    }
    if (candidates.size() == 2) {
        if (Compare()(arr_[candidates[1]], arr_[candidates[0]])) {
            std::swap(candidates[0], candidates[1]);
        }
    }
    std::swap(arr_[index], arr_[candidates[0]]);
    MoveDown(candidates[0]);
}

template <typename T, class Compare>
size_t PriorityQueue<T, Compare>::Size() const {
    return arr_.size();
}

template <typename T, class Compare>
bool PriorityQueue<T, Compare>::Empty() const {
    return arr_.empty();
}

template <typename T, class Compare>
PriorityQueue<T, Compare>& PriorityQueue<T, Compare>::operator=(const PriorityQueue<T, Compare>& other) {
    arr_ = other.arr_;
    return *this;
}

template <typename T, class Compare>
PriorityQueue<T, Compare>& PriorityQueue<T, Compare>::operator=(PriorityQueue<T, Compare>&& other) {
    arr_ = std::move(other.arr_);
    return *this;
}

template <typename T, class Compare>
PriorityQueue<T, Compare>::PriorityQueue(const PriorityQueue<T, Compare>& other) : arr_(other.arr_) {
}

template <typename T, class Compare>
PriorityQueue<T, Compare>::PriorityQueue(PriorityQueue<T, Compare>&& other) : arr_(std::move(other.arr_)) {
}
