#include "FileSystem.h"
#include <cstring>

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
    for(size_t i = 0; i < freeBitmap.size() && result.size() < nbBlocs; i++){
        if(freeBitmap[i]){
            result.push_back(i);
        }

        if(result.size() != nbBlocs){
            return {}; //Retourne le vecteur vide si nous n'avons pas assez de blocs libres
        }

    }
    for(size_t i : result){ // Mettre les valeurs a false pour marquer les blocs comme occuper
        freeBitmap[i] = false; 
    }

    return result;
}



void FileSystem::FreeBlocks(const std::vector<size_t> &blocks)
{
    //Clear the data in each of the blocks
    for(size_t block : blocks){
        freeBitmap[block] = true;
    
        //Placer tout les char occuper a 0
        char zero[1024] = {0};
        device.WriteBlock(block, zero);
    }
}



bool FileSystem::Create(const std::string &filename, size_t sizeInBytes)
{
    //retourne faux si le file existe deja
    if(root.find(filename) != root.end()){
        return false;
    }

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
    //Si le nom du file n'existe pas retourne faux
    if (root.find(filename) == root.end()) {
        return false;
    }


    const size_t blockSize = device.getBlockSize();
    Inode &inode = root[filename];

    //Si on essai d'ajouter plus de donner qu'on peut contenir, retourne faux
    if (offset + data.size() > inode.fileSize) {
        return false;
    }

    size_t dataSize = data.size();
    size_t dataPos = 0;
    size_t currentOffset = offset;

    //Boucle jusqu'a temps qu'il ne reste plus de donnee a ecrire dans le device
    while (dataSize > 0) {
        size_t blockInFile = currentOffset / blockSize;
        size_t blockOffset = currentOffset % blockSize;

        size_t blockInDevice = inode.blockList[blockInFile];

        char buffer[1024]; // Quand je met blockSize donne une erreur pour une raison
        device.ReadBlock(blockInDevice, buffer);

        size_t writable = std::min(blockSize - blockOffset, dataSize);

        std::memcpy(buffer + blockOffset, data.data() + dataPos, writable);
        device.WriteBlock(blockInDevice, buffer);

        dataPos += writable;
        dataSize -= writable;
        currentOffset += writable;
    }

    return true;
}



bool FileSystem::Read(const std::string &filename, size_t offset, size_t length, std::string &outData)
{
    // Vérifier si le fichier existe
    if (root.find(filename) == root.end()) {
        return false;
    }

    Inode &inode = root[filename];
    size_t blockSize = device.getBlockSize();

    // Vérifier que la lecture ne dépasse pas la taille du fichier
    if (offset + length > inode.fileSize) {
        return false;
    }

    outData.clear(); // S'assurer que outData est vide
    size_t restant = length;    
    size_t currentOffset = offset;

    //On boucle jusqu'à la fin
    while (restant > 0) {
        size_t blockInFile = currentOffset / blockSize;
        size_t blockOffset = currentOffset % blockSize;
        size_t blockInDevice = inode.blockList[blockInFile];

        char buffer[1024]; // Assumé 1 Ko
        if (!device.ReadBlock(blockInDevice, buffer)) {
            return false; // Erreur de lecture
        }

        // Nombre d'octets qu'on peut lire à partir du bloc courant
        size_t readable = std::min(blockSize - blockOffset, restant);

        // Ajouter les octets lus à outData
        outData.append(buffer + blockOffset, readable);

        restant -= readable;
        currentOffset += readable;
    }

    return true;
}




bool FileSystem::Delete(const std::string &filename)
{
    if(root.find(filename) == root.end()){
        return false;
    }

    //Clear the data in each of the blocks
    Inode &inode = root[filename];
    FreeBlocks(inode.blockList);

    root.erase(filename);

    return true;
}



void FileSystem::List()
{
    std::cout << "Liste des fichiers :" << std::endl;
    //Parcourt fichiers du répertoire root
    //Pour chaque i-node, affiche : nom du fichier, taille et blocs alloués
    for (const auto &entry : root) {
        const Inode &inode = entry.second;
        std::cout << " Nom - " << inode.fileName
                  << " : taille " << inode.fileSize
                  << ", nbBlocs=" << inode.blockList.size()
                  << std::endl;
    }
}

