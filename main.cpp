#include <iostream>

#include "rangeHash.h"
#include "rangeTree.h"

#include "new.h"

#if 1
typedef RangeTree RangeContainer;
#else
typedef RangeHash RangeContainer;
#endif

int main(int argc, char const *argv[]) {
    RangeContainer rs{{0, 2}, {0, 4}, {0, 8}, {1, 1}, {1, 3}, {1, 4}, {1, 5}};
    rs.insert({0, 6});
    rs.erase(1, 4);
    auto range4 = rs.find(4);
    rs.erase(5);

    return 0;
}
