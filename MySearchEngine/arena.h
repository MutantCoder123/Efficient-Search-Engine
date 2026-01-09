#ifndef ARENA_H
#define ARENA_H
#include<cstdlib>
#include<iostream>

class Arena {
    private:
        char* memblock;
        size_t size;
        size_t offset;
    public:
        // grab big chunk of memory at start to avoid malloc overhead
        Arena(size_t totalsize) {
            size = totalsize;
            memblock = (char*)std::malloc(size);
            offset = 0;
            if(!memblock) {
                std::cerr<<"Error: Memory alloc failed\n";
                std::exit(1);
            }
        }
        
        // bump pointer allocation - O(1) and cache friendly
        void* alloc(size_t needed) {
            if(offset+needed > size) {
                std::cerr<<"Arena full\n";
                std::exit(2);
            }
            void* ptr = memblock + offset;
            offset += needed;
            return ptr;
        }

        int getUsed() {
            return offset;
        }

        ~Arena() {
            std::free(memblock);
        }
};
#endif