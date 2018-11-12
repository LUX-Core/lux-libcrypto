#include "StorageHeap.h"

void StorageHeap::AddChunk(const std::string& path, uint64_t size)
{
    StorageChunk chunk;
    chunk.path = path;
    chunk.totalSpace = size;
    chunk.freeSpace = 0;
    chunks.push_back(chunk);
}

void StorageHeap::FreeChunk(const std::string& path)
{
    for (auto it = chunks.begin(); it != chunks.end(); ++it) {
        if (it->path == path) {
            chunks.erase(it);
            break;
        }
    }
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
                file.pubkey = pubkey;
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
