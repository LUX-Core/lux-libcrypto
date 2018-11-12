#include "StorageHeap.h"

void StorageHeap::AddChunk(const std::string& path, uint64_t size)
{
    StorageChunk chunk;
    chunk.path = path;
    chunk.totalSpace = size;
    chunk.freeSpace = 0;
    cs_dfs.lock();
    {
        chunks.push_back(chunk);
    }
    cs_dfs.unlock();
}

void StorageHeap::FreeChunk(const std::string& path)
{
    cs_dfs.lock();
    {
        for (auto it = chunks.begin(); it != chunks.end(); ++it) {
            if (it->path == path) {
                chunks.erase(it);
                break;
            }
        }
    }
    cs_dfs.unlock();
}

std::vector<StorageChunk> StorageHeap::GetChunks() const
{
    return chunks;
};

uint64_t StorageHeap::MaxAllocateSize() const
{
    uint64_t nMaxAllocateSize = 0;
    for (auto&& chunk : chunks) {
        if (chunk.freeSpace > nMaxAllocateSize) {
            nMaxAllocateSize = chunk.freeSpace;
        }
    }
    return nMaxAllocateSize;
}

void StorageHeap::SetPubKey(const std::string& uri, const std::string& pubkey)
{
    for (auto&& chunk : chunks) {
        for (auto&& file : chunk.files) {
            if (file.uri == uri) {
                cs_dfs.lock();
                {
                    file.pubkey = pubkey;
                }
                cs_dfs.unlock();
                return;
            }
        }
    }
}

boost::optional<AllocatedFile> StorageHeap::GetFile(const std::string& uri) const
{
    auto file_it = files.find(uri);
    if (file_it != files.end())
        return {file_it->second};
    return {};
}

boost::optional<AllocatedFile> StorageHeap::AllocateFile(const std::string& chunkPath, const std::string& uri, uint64_t size)
{
    for (auto&& chunk : chunks) {
        if (chunk.path == chunkPath) {
            if (chunk.freeSpace >= size) {
                AllocatedFile file;
                // file.filename = ; // TODO: generate it
                file.size = size;
                file.uri = uri;
                // add file to heap
                cs_dfs.lock();
                {
                    files[uri] = file;
                    chunk.freeSpace -= size;
                }
                cs_dfs.unlock();
                return file;
            }
            return {};
        }
    }
    return {};
}

void StorageHeap::FreeFile(const std::string& uri)
{
    for (auto&& chunk : chunks) {
        auto&& _files = chunk.files;
        for (auto it = _files.begin(); it != _files.end(); ++it) {
            if (it->uri == uri) {
                cs_dfs.lock();
                {
                    _files.erase(it);
                    files.erase(files.find(uri));
                }
                cs_dfs.unlock();
                return;
            }
        }
    }
}
