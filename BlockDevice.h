//
// NOM PRENOM :
// IDUL :
//

#ifndef BLOCKDEVICE_H
#define BLOCKDEVICE_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

/**
 * BlockDevice simule un disque virtuel de 64 Ko
 * découpé en blocs de 1 Ko.
 */
class BlockDevice {
private:
    std::vector<char> disk;
    const size_t BLOCK_SIZE = 1024; // 1ko
    const size_t NB_BLOCKS = 64; // 64 blocs
    const size_t DISK_SIZE = BLOCK_SIZE * NB_BLOCKS;


public:
    BlockDevice();
    bool ReadBlock(size_t blockIndex, char* buffer);
    bool WriteBlock(size_t blockIndex, const char* data);
   
    size_t getBlockCount() const{
        return NB_BLOCKS;
    }

    size_t getBlockSize() const{
        return BLOCK_SIZE;
    }
};
#endif //BLOCKDEVICE_H
