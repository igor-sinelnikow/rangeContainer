#include "rangeTree.h"

#include <limits>

#include "new.h"

template<class Set>
static void eraseIf(Set& set, typename Set::iterator& it, bool cond) {
    if (cond) {
        it = set.erase(it);
    }
    else {
        ++it;
    }
}

bool LeftOrderRange::operator()(const Range& lhs, const Range& rhs) const {
    if (lhs.start_address < rhs.start_address)
        return true;
    if (lhs.start_address == rhs.start_address)
        return lhs.size < rhs.size;
    return false;
}

bool RightOrderRange::operator()(const Range& lhs, const Range& rhs) const {
    const auto lhs_last = lhs.last_address();
    const auto rhs_last = rhs.last_address();
    return lhs_last < rhs_last || (lhs_last == rhs_last && lhs.size < rhs.size);
}

void RangeTree::clear() noexcept {
    rangesLeftOrder_.clear();
    rangesRightOrder_.clear();
}

void RangeTree::insert(const Range& r) {
    rangesLeftOrder_.insert(r);
    rangesRightOrder_.insert(r);
}

size_t RangeTree::erase(uintptr_t address) {
    const auto lEnd = rangesLeftOrder_.upper_bound({address, std::numeric_limits<size_t>::max()});
    auto lIt = rangesLeftOrder_.begin();
    if (lEnd == lIt) {
        return 0;
    }

    auto rIt = rangesRightOrder_.lower_bound({address, 0});
    const auto rEnd = rangesRightOrder_.end();
    if (rIt == rEnd) {
        return 0;
    }

    const auto count = size();
    if (lEnd == rangesLeftOrder_.end() && rIt == rangesRightOrder_.begin()) {
        clear();
        return count;
    }

    while (lIt != lEnd && rIt != rEnd) {
        eraseIf(rangesLeftOrder_, lIt, address < lIt->last_address());
        eraseIf(rangesRightOrder_, rIt, rIt->start_address <= address);
    }
    while (lIt != lEnd) {
        eraseIf(rangesLeftOrder_, lIt, address < lIt->last_address());
    }
    while (rIt != rEnd) {
        eraseIf(rangesRightOrder_, rIt, rIt->start_address <= address);
    }

    return count - size();
}

size_t RangeTree::erase(uintptr_t start_address, size_t size) {
    return erase({start_address, size});
}

size_t RangeTree::erase(const Range& r) {
    if (rangesLeftOrder_.erase(r))
        return rangesRightOrder_.erase(r);
    return 0;
}

std::vector<Range> RangeTree::find(uintptr_t address) const {
    const auto lEnd = rangesLeftOrder_.upper_bound({address, std::numeric_limits<size_t>::max()});
    auto lIt = rangesLeftOrder_.begin();
    if (lEnd == lIt) {
        return {};
    }

    auto rIt = rangesRightOrder_.lower_bound({address, 0});
    const auto rEnd = rangesRightOrder_.end();
    if (rIt == rEnd) {
        return {};
    }

    if (lEnd == rangesLeftOrder_.end() && rIt == rangesRightOrder_.begin()) {
        return {lIt, lEnd};
    }

    std::vector<Range> lVec;
    std::vector<Range> rVec;
    for (;;) {
        auto range = *lIt;
        if (address < range.last_address()) {
            lVec.push_back(std::move(range));
        }
        if (++lIt == lEnd) {
            return lVec;
        }

        range = *rIt;
        if (range.start_address <= address) {
            rVec.push_back(std::move(range));
        }
        if (++rIt == rEnd) {
            return rVec;
        }
    }
}
