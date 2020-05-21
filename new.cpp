#include <iomanip>
#include <iostream>
#include <unordered_map>

#include "new.h"
#undef new

struct MemCtl
{
    std::string file;
    int line;
    size_t size;
};

class MemMap
{
public:
    MemMap() = default;
    ~MemMap()
    {
        beingDestructed = true;

        std::cout << "====" << std::endl;
        std::cout << "==== Memory leak check" << std::endl;
        std::cout << "====" << std::endl;
        if (m.empty()) {
            std::cout << "==== No memory leaks" << std::endl;
            return;
        }

        std::cout << "==== Memory leaks:" << std::endl;
        for (const auto& [p, detail] : m) {
            std::cout << detail.file << ",\t" << detail.line << ",\t" << detail.size << ",\t0x";
            const auto* q = reinterpret_cast<int8_t*>(p);
            for (size_t i = 0; i < 20; ++i) {
                std::cout << std::hex << std::setw(2) << std::setfill('0') << int(q[i]);
            }
            std::cout << std::dec << std::endl;
        }
    }

    bool beingDestructed = false;
    std::unordered_map<void*, MemCtl> m;
};

static MemMap mm;

void* operator new(size_t size)
{
    if (void* p = std::malloc(size))
        return p;
    else
        throw std::bad_alloc{};
}

void operator delete(void* p) noexcept
{
    if (!mm.beingDestructed)
        mm.m.erase(p);
    std::free(p);
}

void* operator new[](size_t size)
{
    if (void* p = std::malloc(size))
        return p;
    else
        throw std::bad_alloc{};
}

void operator delete[](void* p) noexcept
{
    mm.m.erase(p);
    std::free(p);
}

void* operator new(size_t size, std::string file, int line)
{
    void* p = ::operator new(size);
    mm.m[p] = {std::move(file), line, size};
    return p;
}

void operator delete(void* p, std::string file, int line)
{
    mm.m.erase(p);
    ::operator delete(p);
}

void* operator new[](size_t size, std::string file, int line)
{
    void* p = ::operator new[](size);
    mm.m[p] = {std::move(file), line, size};
    return p;
}

void operator delete[](void* p, std::string file, int line)
{
    mm.m.erase(p);
    ::operator delete[](p);
}
