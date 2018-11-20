#include "CryptoSplitter.h"

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <boost/test/unit_test.hpp>
#include <iostream>

BOOST_AUTO_TEST_SUITE(crypto_splitter_tests)

BOOST_AUTO_TEST_CASE(get_crypto_replica_size)
{
    const int blockSize = 128;
    const int srcSize = 2000;
    RSA *rsa = RSA_generate_key(1024, 3, nullptr, nullptr);

    uint64_t replicaSize = GetCryptoReplicaSize(srcSize , {rsa, blockSize});

    BOOST_CHECK_EQUAL(replicaSize, 2048);
    RSA_free(rsa);
}

BOOST_AUTO_TEST_CASE(encrypt_block)
{
    const int blockSize = 128;
    RSA *rsa = RSA_generate_key(1024, 3, nullptr, nullptr);
    const byte src[] = "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678"; // length 64
    const char chAESKey[] = "1234567890123456";
    const AESKey aesKey(chAESKey, chAESKey + sizeof(chAESKey)/sizeof(*chAESKey));
    const uint64_t offset = 1;
    byte cipherText[128];
    byte plainText[128];
    byte temp[128];
    const AESSettings aes = {aesKey, 16};
    const RSASettings rsaSettings = {rsa, blockSize};

    EncryptData(src, offset, 128, cipherText, temp, aes, rsaSettings);
    DecryptData(cipherText, offset, 128, plainText, temp, aes, rsaSettings);

    BOOST_CHECK_EQUAL(memcmp(src, plainText, 128 * sizeof(char)), 0);
    RSA_free(rsa);
}

BOOST_AUTO_TEST_SUITE_END()
