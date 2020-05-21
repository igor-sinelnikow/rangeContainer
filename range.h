#pragma once

#include <cstdint>

struct Range
{
    uintptr_t start_address;
    size_t size;

    uintptr_t last_address() const {
        return start_address + size;
    }
};
