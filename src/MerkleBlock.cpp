#include <openssl/sha.h>

#include "MerkleBlock.h"

/** Compute the 256-bit hash of a void pointer */
inline void Hash(void* in, unsigned int len, unsigned char* out)
{
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, in, len);
    SHA256_Final(out, &sha256);
}

void ConstructMerkleTreeLayer(std::ifstream &prevLayer, uint64_t size, std::ofstream &outputLayer)
{
    using namespace std;

    if(!prevLayer.good() || !outputLayer.good()){
        // throw exception
        return;
    }
    unsigned char data[2 * SHA256_DIGEST_LENGTH];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    for (uint64_t i = 1; i < size; i+=2) {
        prevLayer.read((char *)data, 2 * SHA256_DIGEST_LENGTH);
        Hash(data, 2 * SHA256_DIGEST_LENGTH, hash);
        outputLayer.write((char *)hash, SHA256_DIGEST_LENGTH);
    }

    if(size % 2) {
        prevLayer.read((char *)hash, SHA256_DIGEST_LENGTH);
        outputLayer.write((char *)hash, SHA256_DIGEST_LENGTH);
    }
}

void ConstructMerkleTree(std::ifstream &firstLayer, uint64_t size, std::ofstream &outputStream)
{
    for (uint64_t currentSize = size; currentSize > 1; currentSize = currentSize / 2 + currentSize % 2) {
        ConstructMerkleTreeLayer(firstLayer, currentSize, outputStream);
    }
}


void ConstructMerklePath(std::ifstream &firstLayer, uint64_t size, std::vector<uint256> path)
{

}
