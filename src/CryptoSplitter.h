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

typedef unsigned char byte;
typedef std::vector<byte> AESKey;

static constexpr size_t nBlockSizeRSA = 128;
static constexpr size_t nBlockSizeAES = 16;

BIGNUM *GetMinModulus();
uint64_t GetCryptoReplicaSize(size_t srcSize, size_t blockSizeRSA = nBlockSizeRSA);
void EncryptData(const byte *src, uint64_t offset, size_t srcSize, byte *cipherText,
                 const AESKey &aesKey, RSA *rsa);
void DecryptData(const byte *src, uint64_t offset, size_t srcSize, byte *plainText,
                 const AESKey &aesKey, RSA *rsa);



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
