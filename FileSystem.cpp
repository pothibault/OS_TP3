#include "FileSystem.h"

FileSystem::FileSystem(BlockDevice &dev) : device(dev)
{
    size_t blockCount = dev.getBlockCount();
    freeBitmap.resize(blockCount, true);  //Init le bitmap avec toute les places libre
}



void FileSystem::Compact()
{
}



std::vector<size_t> FileSystem::AllocateBlocks(size_t nbBlocs)
{
    std::vector<size_t> result;

    //Boucle a travers le bitmap pour trouver des espaces libre
    for(size_t i = 0; i < freeBitmap.size(); i++){
        if(freeBitmap[i]){
            result.push_back(i);
            freeBitmap[i] = false;
        }

        if(result.size() == nbBlocs){
            return result; //Retourne la liste de bloc si nous avons trouvez tout les place
        }

    }
    for(size_t i : result){ // Si nous ne trouvons pas assez de place, on remet les place a true puisqu'il ne sont pas utiliser
        freeBitmap[i] = true; 
    }

    return {};
}



void FileSystem::FreeBlocks(const std::vector<size_t> &blocks)
{
}



bool FileSystem::Create(const std::string &filename, size_t sizeInBytes)
{
    //retourne faux si le file existe deja
    if(root.find(filename) != root.end()){
        return false;
    }

    size_t blockSize = device.getBlockSize();

    //Calcule le nombre de blocs necessaire
    size_t blockSize = device.getBlockSize();
    size_t nbBlocs = (sizeInBytes + blockSize - 1) / blockSize;

    //Allocate les blocs necessaire
    std::vector<size_t> allocatedBlocs = AllocateBlocks(nbBlocs);
    if(allocatedBlocs.empty()){ // Si l'allocation fail (list vide) retourne faux
        return false;
    }

    //creer le inode
    Inode inode;
    inode.fileName = filename;
    inode.fileSize = sizeInBytes;
    inode.blockList = allocatedBlocs;

    root[filename] = inode;
    
    return true;
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
