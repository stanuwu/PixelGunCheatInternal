#include "Scanner.h"

std::vector<uint64_t> BKCScanner::scan(std::vector<int> pattern) {

    std::vector<uint64_t> results;
    MEMORY_BASIC_INFORMATION mbi;

    for (uint64_t address = 0; VirtualQuery((void*)address, &mbi, sizeof(mbi)); address += mbi.RegionSize) { // iterate through memory regions
        if (mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS) { // filer them out to only the useful ones :3

            uint64_t endAddress = address + mbi.RegionSize;

            for (uint64_t i = address; i < endAddress - pattern.size(); i++) { // iterate through the memory region

                bool found = true;

                uintptr_t ptr = i;
                for (int j = 0; j < pattern.size(); j++) { // iterate through the pattern to check if it matches
                    if (pattern[j] != -1 && *(unsigned char*)(ptr++) != pattern[j]) {
                        found = false;
                        break;
                    }
                }
                if (found) results.push_back(i); // yippeee ~w~
            }
        }
    }
    return results;
}