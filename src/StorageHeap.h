#ifndef LUX_LIB_CRYPTO_STORAGE_HEAP_H
#define LUX_LIB_CRYPTO_STORAGE_HEAP_H

#include <map>
#include <vector>
#include <string>
#include <mutex>
#include <boost/optional.hpp>

struct AllocatedFile
{
    std::string filename;
    uint64_t size;
    std::string uri;
    std::string pubkey;
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
protected:
    mutable std::mutex cs_dfs;
    std::vector<StorageChunk> chunks;
    std::map<std::string, AllocatedFile> files;

public:
    virtual ~StorageHeap() {}
    void AddChunk(const std::string& path, uint64_t size);
    void FreeChunk(const std::string& path);
    std::vector<StorageChunk> GetChunks() const;
    uint64_t MaxAllocateSize() const;
    boost::optional<AllocatedFile> AllocateFile(const std::string& uri, uint64_t size);
    void FreeFile(const std::string& uri);
    boost::optional<AllocatedFile> GetFile(const std::string& uri) const;
    void SetPubKey(const std::string& uri, const std::string& pubkey);
};

#endif //LUX_LIB_CRYPTO_STORAGE_HEAP_H
