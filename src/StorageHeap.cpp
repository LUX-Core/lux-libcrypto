#include "StorageHeap.h"

void StorageHeap::AddChunk(const std::string& path, uint64_t size)
{
    std::lock_guard<std::mutex> scoped_lock(cs_dfs);
    StorageChunk chunk;
    chunk.path = path;
    chunk.totalSpace = size;
    chunk.freeSpace = size;
    chunks.push_back(chunk);
}

void StorageHeap::FreeChunk(const std::string& path)
{
    std::lock_guard<std::mutex> scoped_lock(cs_dfs);
    for (auto it = chunks.begin(); it != chunks.end(); ++it) {
        if (it->path == path) {
            chunks.erase(it);
            break;
        }
    }
}

std::vector<StorageChunk> StorageHeap::GetChunks() const
{
    std::lock_guard<std::mutex> scoped_lock(cs_dfs);
    return chunks;
};

uint64_t StorageHeap::MaxAllocateSize() const
{
    std::lock_guard<std::mutex> scoped_lock(cs_dfs);
    uint64_t nMaxAllocateSize = 0;
    for (auto&& chunk : chunks) {
        if (chunk.freeSpace > nMaxAllocateSize) {
            nMaxAllocateSize = chunk.freeSpace;
        }
    }
    return nMaxAllocateSize;
}

boost::optional<AllocatedFile> StorageHeap::AllocateFile(const std::string& chunkPath, const std::string& uri, uint64_t size)
{
    std::lock_guard<std::mutex> scoped_lock(cs_dfs);
    for (auto&& chunk : chunks) {
        if (chunk.path == chunkPath) {
            if (chunk.freeSpace >= size) {
                AllocatedFile file;
                // file.filename = ; // TODO: generate it
                file.size = size;
                file.uri = uri;
                // add file to heap
                files[uri] = file;
                chunk.freeSpace -= size;
                return file;
            }
            return {};
        }
    }
    return {};
}

void StorageHeap::FreeFile(const std::string& uri)
{
    std::lock_guard<std::mutex> scoped_lock(cs_dfs);
    for (auto&& chunk : chunks) {
        auto&& _files = chunk.files;
        for (auto it = _files.begin(); it != _files.end(); ++it) {
            if (it->uri == uri) {
                _files.erase(it);
                files.erase(files.find(uri));
                return;
            }
        }
    }
}

boost::optional<AllocatedFile> StorageHeap::GetFile(const std::string& uri) const
{
    std::lock_guard<std::mutex> scoped_lock(cs_dfs);
    auto file_it = files.find(uri);
    if (file_it != files.end())
        return {file_it->second};
    return {};
}

void StorageHeap::SetPubKey(const std::string& uri, const std::string& pubkey)
{
    std::lock_guard<std::mutex> scoped_lock(cs_dfs);
    for (auto&& chunk : chunks) {
        for (auto&& file : chunk.files) {
            if (file.uri == uri) {
                file.pubkey = pubkey;
                return;
            }
        }
    }
}

