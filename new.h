#pragma once

void* operator new(size_t size);
void operator delete(void* p) noexcept;

void* operator new[](size_t size);
void operator delete[](void* p) noexcept;

void* operator new(size_t size, std::string file, int line);
void operator delete(void* p, std::string file, int line);

void* operator new[](size_t size, std::string file, int line);
void operator delete[](void* p, std::string file, int line);

#define new new(__FILE__, __LINE__)
