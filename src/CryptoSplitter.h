#ifndef LUX_LIB_CRYPTO_CRYPTOSPLITTER_H
#define LUX_LIB_CRYPTO_CRYPTOSPLITTER_H

#include <string>
#include <vector>
//#include <map>
//#include <mutex>
//#include <boost/optional.hpp>
//#include <memory>
#include <openssl/bn.h>

struct AllocatedFile;

using byte = unsigned char;
using AESKey = std::vector<byte>;

struct AESSettings {
    AESKey key;
    size_t blockSize;
};

struct RSASettings {
    RSA *keypair;
    size_t blockSize;
};

uint64_t GetCryptoReplicaSize(size_t srcSize, const RSASettings& rsa);
void EncryptData(const byte* src, uint64_t offset, size_t srcSize, byte *cipherText,
                 const AESSettings& aes, const RSASettings& rsa);
void DecryptData(const byte* src, uint64_t offset, size_t srcSize, byte *plainText,
                 const AESSettings& aes, const RSASettings& rsa);

//void EncryptFile(const std::string& source,
//                 const std::string& pathToDestination,
//                 const AESSettings& aes,
//                 const RCASettings& rca,
//                 const AllocatedFile& distFileInfo);
//void DecryptFile(const std::string& pathToSource,
//                 const std::string& destination,
//                 const AESSettings& aes,
//                 const RSASettings& rca,
//                 const AllocatedFile& sourceFileInfo);

#endif //LUX_LIB_CRYPTO_CRYPTOSPLITTER_H
