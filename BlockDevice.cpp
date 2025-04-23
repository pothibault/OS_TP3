#include "BlockDevice.h"
#include <cstring>

BlockDevice::BlockDevice()
{
    disk.resize(DISK_SIZE, 0); //vecteur de 65536 octets
}

bool BlockDevice::ReadBlock(size_t blockIndex, char *buffer)
{
    if (blockIndex >= 0 && blockIndex < NB_BLOCKS) {
        size_t offset = blockIndex * BLOCK_SIZE;
        std::memcpy(buffer, &disk[offset], BLOCK_SIZE); // memcpy(pointeur destination, adresse debut bloc, quantité d'octets à copier)
        return true;

    } 
    else {return false;}
}

bool BlockDevice::WriteBlock(size_t blockIndex, const char *data)
{
    if (blockIndex >= 0 && blockIndex < NB_BLOCKS) {
        size_t offset = blockIndex * BLOCK_SIZE;
        std::memcpy(&disk[offset], data , BLOCK_SIZE); // memcpy(pointeur destination, adresse source , quantité d'octets à copier)
        

        return true;
    } 
    else {return false;}
}




