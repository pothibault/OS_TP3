#include "FileSystem.h"

FileSystem::FileSystem(BlockDevice &dev)
{
}



void FileSystem::Compact()
{
}



std::vector<size_t> FileSystem::AllocateBlocks(size_t nbBlocs)
{
    return std::vector<size_t>();
}



void FileSystem::FreeBlocks(const std::vector<size_t> &blocks)
{
}



bool FileSystem::Create(const std::string &filename, size_t sizeInBytes)
{
    return false;
}



bool FileSystem::Write(const std::string &filename, size_t offset, const std::string &data)
{
    return false;
}



bool FileSystem::Read(const std::string &filename, size_t offset, size_t length, std::string &outData)
{
    return false;
}



bool FileSystem::Delete(const std::string &filename)
{
    return false;
}



void FileSystem::List()
{
}
