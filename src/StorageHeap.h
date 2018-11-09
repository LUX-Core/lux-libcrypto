#ifndef LUX_LIB_CRYPTO_STORAGEHEAP_H
#define LUX_LIB_CRYPTO_STORAGEHEAP_H

#include <map>
#include <vector>
#include <string>

typedef signed long long        int64_t;
typedef unsigned long long      uint64_t;

struct AllocatedFile
{
    std::string path;

    uint64_t size;

    std::string uri;

    unsigned char* pubkey;
};


struct StorageChunk
{
    std::string path;

    std::vector<AllocatedFile> files;

    uint64_t totalSpace;

    uint64_t freeSpace;
};


class StorageHeap
{
private:
    std::vector<StorageChunk> chunks;

    std::map<std::string, AllocatedFile> files;

public:
    void AddChunk(const std::string path, const uint64_t size);

    void FreeChunk(const std::string path);

    std::vector<StorageChunk> GetChunks;

    uint64_t MaxAllocateSize();

    void SetPubKey(const std::string uri, const unsigned char* pubkey);

    boost::optional<AllocatedFile> GetFile(const std::string uri);

    boost::optional<AllocatedFile> AllocateFile(const std::string uri);

    void FreeFile(const std::string uri);
};

#endif //LUX_LIB_CRYPTO_STORAGEHEAP_H
