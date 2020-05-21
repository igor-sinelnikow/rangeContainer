#include "rangeHash.h"

#include "new.h"

bool operator==(const Range& lhs, const Range& rhs) {
    return lhs.start_address == rhs.start_address && lhs.size == rhs.size;
}

void RangeHash::clear() noexcept {
    addrMap_.clear();
    ranges_.clear();
    lastId_ = 0;
}

void RangeHash::insert(const Range& r) {
    const auto last_address = r.last_address();
    for (auto addr = r.start_address; addr < last_address; ++addr) {
        addrMap_[addr].insert(lastId_);
    }
    ranges_.insert({lastId_++, r});
}

size_t RangeHash::erase(uintptr_t address) {
    const auto it = addrMap_.find(address);
    if (it == addrMap_.end()) {
        return 0;
    }

    const auto& ids = it->second;
    const auto count = ids.size();
    if (count == size()) {
        clear();
        return count;
    }

    std::vector<size_t> idsToErase{ids.begin(), ids.end()};
    for (const auto& id : idsToErase) {
        const auto pos = ranges_.find(id);
        const auto& r = pos->second;
        const auto last_address = r.last_address();
        for (auto addr = r.start_address; addr < last_address; ++addr) {
            addrMap_[addr].erase(id);
        }
        ranges_.erase(pos);
    }
    return count;
}

size_t RangeHash::erase(uintptr_t start_address, size_t size) {
    return erase({start_address, size});
}

size_t RangeHash::erase(const Range& r) {
    const auto it = addrMap_.find(r.start_address);
    if (it == addrMap_.end()) {
        return 0;
    }

    const auto currSize = size();
    std::vector<size_t> idsToErase;
    for (const auto& id : it->second) {
        const auto pos = ranges_.find(id);
        const auto& range = pos->second;
        if (range == r) {
            idsToErase.push_back(id);
            ranges_.erase(pos);
        }
    }

    const auto count = idsToErase.size();
    if (!count) {
        return 0;
    }
    if (count == currSize) {
        clear();
        return count;
    }

    const auto last_address = r.last_address();
    for (auto addr = r.start_address; addr < last_address; ++addr) {
        auto& ids = addrMap_[addr];
        for (const auto& id : idsToErase) {
            ids.erase(id);
        }
    }
    return count;
}

std::vector<Range> RangeHash::find(uintptr_t address) const {
    const auto it = addrMap_.find(address);
    if (it == addrMap_.end()) {
        return {};
    }

    const auto& ids = it->second;
    std::vector<Range> ret;
    ret.reserve(ids.size());
    for (const auto& id : ids) {
        ret.push_back(ranges_.at(id));
    }
    return ret;
}
