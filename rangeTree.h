#pragma once

#include <set>
#include <vector>

#include "range.h"

struct LeftOrderRange
{
    bool operator()(const Range& lhs, const Range& rhs) const;
};

struct RightOrderRange
{
    bool operator()(const Range& lhs, const Range& rhs) const;
};

class RangeTree
{
public:
    RangeTree() = default;

    RangeTree(const RangeTree& other) = default;

    RangeTree(RangeTree&& other) = default;

    template<class InputIt>
    RangeTree(InputIt first, InputIt last);

    RangeTree(std::initializer_list<Range> init): RangeTree(init.begin(), init.end()) {}

    ~RangeTree() = default;

    [[nodiscard]] bool empty() const noexcept { return rangesLeftOrder_.empty(); }

    size_t size() const noexcept { return rangesLeftOrder_.size(); }

    size_t max_size() const noexcept { return rangesLeftOrder_.max_size(); }

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
    std::set<Range, LeftOrderRange> rangesLeftOrder_;
    std::set<Range, RightOrderRange> rangesRightOrder_;
};

template<class InputIt>
RangeTree::RangeTree(InputIt first, InputIt last): RangeTree() {
    insert(first, last);
}

template<class InputIt>
void RangeTree::insert(InputIt first, InputIt last) {
    while (first != last) {
        insert(*first++);
    }
}
