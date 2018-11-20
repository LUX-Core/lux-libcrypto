#include "CryptoSplitter.h"

//#include <iostream>
//#include <fstream>
//#include <sstream>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <cmath>

void handleErrors()
{
    ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(const unsigned char *plaintext, int plaintext_len, AESKey key,
            const unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    /* Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key.data(), iv))
        handleErrors();

    /* Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /* Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

uint64_t GetCryptoReplicaSize(size_t srcSize, const RSASettings& rsa){
    return (uint64_t)ceil((double)srcSize / rsa.blockSize) * rsa.blockSize;
}

void ShuffleAES(const byte* src, uint64_t offset, size_t srcSize, byte *cipherText, const AESSettings& aes)
{
    constexpr size_t chunkSize = 128;
    uint64_t salt[chunkSize / sizeof(uint64_t)];
    for (size_t k = 0; k < srcSize; k += chunkSize){
        // build counter-based salt
        memset(salt, 0, chunkSize);
        for (uint64_t i = 0; i < chunkSize / aes.blockSize; ++i) {
            // if aes.blockSize is 16, then salt is [0, offset, 0, offset + 1, 0, offset + 2...]
            const size_t step = aes.blockSize / sizeof(uint64_t);
            salt[(i + 1) * step - 1] = offset + i;
        }
        // encrypt salt
        byte encryptSalt[chunkSize + 16];
        if (encrypt((byte*) salt, chunkSize, aes.key, NULL, encryptSalt) != (chunkSize + 16)) {
            handleErrors();
        }
        // xor with salt
        for (size_t i = 0; (i < chunkSize) && ((i + k) < srcSize); ++i){
            cipherText[i + k] =  src[i + k] ^ encryptSalt[i];
        }
    }
}


void EncryptData(const byte* src, uint64_t offset, size_t srcSize, byte *cipherText, byte *temp,
                 const AESSettings& aes, const RSASettings& rsa)
{
    ShuffleAES(src, offset, srcSize, temp, aes);
    auto size = (int)GetCryptoReplicaSize(srcSize, rsa);
    if(RSA_private_encrypt(size, temp, cipherText, rsa.keypair, RSA_NO_PADDING) == -1) {
        ERR_load_crypto_strings();
        handleErrors();
    }
}

void DecryptData(const byte* src, uint64_t offset, size_t srcSize, byte *plainText, byte *temp,
                 const AESSettings& aes, const RSASettings& rsa)
{
    auto size = (int)GetCryptoReplicaSize(srcSize, rsa);
    if(RSA_public_decrypt(size, src, temp, rsa.keypair, RSA_NO_PADDING) == -1) {
        ERR_load_crypto_strings();
        handleErrors();
    }
    ShuffleAES(temp, offset, srcSize, plainText, aes);
}

//#include "StorageHeap.h"
//
//void EncryptFile(const std::string& source,
//                 const std::string& pathToDestination,
//                 const AESSettings& aes,
//                 const RSASettings& rca,
//                 const AllocatedFile& distFileInfo)
//{
//    using namespace std;
//    ifstream sourceFile(source, ios::binary|ios::ate);
//    if(!sourceFile.is_open() || aes.size == 0 || rca.size == 0) {
//        return;
//    }
//    auto sourceFileLength = (uint64_t)sourceFile.tellg();
//    uint64_t position = 0;
//    sourceFile.seekg(0, sourceFile.beg);
//    lock_guard<std::mutex> scoped_lock(distFileInfo.cs);
//    ofstream destFile(pathToDestination + "/" + distFileInfo.filename, ios::binary);
//    if(!destFile.is_open()) {
//        throw invalid_argument("cann't open destination file");;
//    }
//    auto *buffer = (char *)malloc(aes.size * sizeof(char));
//    while(position <= sourceFileLength){
//
//        position += aes.size;
//    }
//}
