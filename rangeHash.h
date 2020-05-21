#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "range.h"

class RangeHash
{
public:
    RangeHash() = default;

    RangeHash(const RangeHash& other) = default;

    RangeHash(RangeHash&& other) = default;

    template<class InputIt>
    RangeHash(InputIt first, InputIt last);

    RangeHash(std::initializer_list<Range> init): RangeHash(init.begin(), init.end()) {}

    ~RangeHash() = default;

    [[nodiscard]] bool empty() const noexcept { return ranges_.empty(); }

    size_t size() const noexcept { return ranges_.size(); }

    size_t max_size() const noexcept { return ranges_.max_size(); }

    void clear() noexcept;

    void insert(const Range& r);

    template<class InputIt>
    void insert(InputIt first, InputIt last);

    void insert(std::initializer_list<Range> ilist) { insert(ilist.begin(), ilist.end()); }

    size_t erase(uintptr_t address);

    size_t erase(uintptr_t start_address, size_t size);

    size_t erase(const Range& r);

    std::vector<Range> find(uintptr_t address) const;

private:
    std::unordered_map<uintptr_t, std::unordered_set<size_t>> addrMap_;
    std::unordered_map<size_t, Range> ranges_;
    size_t lastId_ = 0;
};

template<class InputIt>
RangeHash::RangeHash(InputIt first, InputIt last): RangeHash() {
    insert(first, last);
}

template<class InputIt>
void RangeHash::insert(InputIt first, InputIt last) {
    while (first != last) {
        insert(*first++);
    }
}
