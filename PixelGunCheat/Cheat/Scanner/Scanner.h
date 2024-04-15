#ifndef SCANNER_H
#define SCANNER_H

#include <vector>
#include <Windows.h>

class BKCScanner {
public:
    std::vector<uint64_t> scan(std::vector<int> pattern);
private:
};

#endif
