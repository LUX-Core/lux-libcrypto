#ifndef LUX_LIB_CRYPTO_MERKLEBLOCK_H
#define LUX_LIB_CRYPTO_MERKLEBLOCK_H

#include <fstream>
#include <vector>

template <unsigned int BITS>
class base_uint {
protected:
    enum {
        WIDTH = BITS / 32
    };
    uint32_t pn[WIDTH];
};
class uint256 : public base_uint<256>{};

void ConstructMerkleTreeLayer(std::ifstream &prevLayer, uint64_t size, std::ofstream &outputLayer);
void ConstructMerkleTree(std::ifstream &firstLayer, uint64_t size, std::ofstream &outputStream);
void ConstructMerklePath(std::ifstream &firstLayer, uint64_t size, std::vector<uint256> path);

#endif //LUX_LIB_CRYPTO_MERKLEBLOCK_H
