#include "StorageHeap.h"

#define BOOST_TEST_MODULE Lux Lib Crypto Test

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(storage_heap_tests)

class TestStorageHeap : public StorageHeap {
public:
    std::vector<StorageChunk>& _chunks() {
        return chunks;
    }

    std::map<std::string, AllocatedFile>& _files() {
        return files;
    }
};

BOOST_AUTO_TEST_CASE(add_chunk)
{
    StorageHeap heap;
    uint64_t size = 100ull * 1024 * 1024 * 1024; // 100 Gb
    std::string path = "/path/to/some/directory";
    heap.AddChunk(path, size);

    auto *pTestHeap = static_cast<TestStorageHeap*>(&heap);
    BOOST_CHECK(pTestHeap->_chunks().size() == 1);

    StorageChunk chunk = pTestHeap->_chunks()[0];
    BOOST_CHECK(chunk.path == path && \
        chunk.totalSpace == size && \
        chunk.freeSpace == 0 && \
        chunk.files.size() == 0 \
    );
}

BOOST_AUTO_TEST_SUITE_END()
