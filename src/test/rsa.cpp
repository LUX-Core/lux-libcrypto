
#include <boost/test/unit_test.hpp>
#include <openssl/pem.h>
#include <iostream>
#include <thread>

BOOST_AUTO_TEST_SUITE(storage_heap_tests)

RSA* loadPUBLICKeyFromString( const char* publicKeyStr )
{
    // A BIO is an I/O abstraction (Byte I/O?)

    // BIO_new_mem_buf: Create a read-only bio buf with data
    // in string passed. -1 means string is null terminated,
    // so BIO_new_mem_buf can find the dataLen itself.
    // Since BIO_new_mem_buf will be READ ONLY, it's fine that publicKeyStr is const.
    BIO* bio = BIO_new_mem_buf( (void*)publicKeyStr, -1 ) ; // -1: assume string is null terminated

    BIO_set_flags( bio, BIO_FLAGS_BASE64_NO_NL ) ; // NO NL

    // Load the RSA key from the BIO
    RSA* rsaPubKey = PEM_read_bio_RSA_PUBKEY( bio, NULL, NULL, NULL ) ;
    if( !rsaPubKey )
        printf( "ERROR: Could not load PUBLIC KEY!  PEM_read_bio_RSA_PUBKEY FAILED\n") ;

    BIO_free( bio ) ;
    return rsaPubKey ;
}

RSA* loadPRIVATEKeyFromString( const char* privateKeyStr )
{
    BIO *bio = BIO_new_mem_buf( (void*)privateKeyStr, -1 );
    //BIO_set_flags( bio, BIO_FLAGS_BASE64_NO_NL ) ; // NO NL
    RSA* rsaPrivKey = PEM_read_bio_RSAPrivateKey( bio, NULL, NULL, NULL ) ;

    if ( !rsaPrivKey )
        printf("ERROR: Could not load PRIVATE KEY!  PEM_read_bio_RSAPrivateKey FAILED\n");

    BIO_free( bio ) ;
    return rsaPrivKey ;
}

void test() {
    // RSA_public_encrypt(int flen, const unsigned char *from, unsigned char *to, RSA *rsa, int padding);
    int flen = 128;
    std::string data;
    std::string out;
    std::string out2;
    data.resize((unsigned long)flen);
    out.resize((unsigned long)flen);
    out2.resize((unsigned long)flen);
    memset((void *)data.data(), 'A', sizeof(char) * flen);

    const char *b64_pKey = "-----BEGIN PUBLIC KEY-----\n"
            "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCp2w+8HUdECo8V5yuKYrWJmUbL\n"
            "tD6nSyVifN543axXvNSFzQfWNOGVkMsCo6W4hpl5eHv1p9Hqdcf/ZYQDWCK726u6\n"
            "hsZA81AblAOOXKaUaxvFC+ZKRJf+MtUGnv0v7CrGoblm1mMC/OQI1JfSsYi68Epn\n"
            "aOLepTZw+GLTnusQgwIDAQAB\n"
            "-----END PUBLIC KEY-----\n";

    // private key
    const char *b64priv_key = "-----BEGIN RSA PRIVATE KEY-----\n"
            "MIICXAIBAAKBgQCp2w+8HUdECo8V5yuKYrWJmUbLtD6nSyVifN543axXvNSFzQfW\n"
            "NOGVkMsCo6W4hpl5eHv1p9Hqdcf/ZYQDWCK726u6hsZA81AblAOOXKaUaxvFC+ZK\n"
            "RJf+MtUGnv0v7CrGoblm1mMC/OQI1JfSsYi68EpnaOLepTZw+GLTnusQgwIDAQAB\n"
            "AoGBAKDuq3PikblH/9YS11AgwjwC++7ZcltzeZJdGTSPY1El2n6Dip9ML0hUjeSM\n"
            "ROIWtac/nsNcJCnvOnUjK/c3NIAaGJcfRPiH/S0Ga6ROiDfFj2UXAmk/v4wRRUzr\n"
            "5lsA0jgEt5qcq2Xr/JPQVGB4wUgL/yQK0dDhW0EdrJ707e3BAkEA1aIHbmcVfCP8\n"
            "Y/uWuK0lvWxrIWfR5MlHhI8tD9lvkot2kyXiV+jB6/gktwk1QaFsy7dCXn7w03+k\n"
            "xrjEGGN+kQJBAMuKf55lDtU9K2Js3YSStTZAXP+Hz7XpoLxmbWFyGvBx806WjgAD\n"
            "624irwS+0tBxkERbRcisfb2cXmAx8earT9MCQDZuVCpjBWxd1t66qYpgQ29iAmG+\n"
            "jBIY3qn9uOOC6RSTiCCx1FvFqDMxRFmGdRVFxeyZwsVE3qNksF0Zko0MPKECQCEe\n"
            "oDV97DP2iCCz5je0R5hUUM2jo8DOC0GcyR+aGZgWcqjPBrwp5x08t43mHxeb4wW8\n"
            "dFZ6+trnntO4TMxkA9ECQB+yCPgO1zisJWYuD46KISoesYhwHe5C1BQElQgi9bio\n"
            "U39fFo88w1pok23a2CZBEXguSvCvexeB68OggdDXvy0=\n"
            "-----END RSA PRIVATE KEY-----\n";

    RSA *rsa = loadPUBLICKeyFromString( b64_pKey );
    RSA *privKey = loadPRIVATEKeyFromString( b64priv_key );

    for (int i = 0; i < 500000 / 4; ++i) {
        RSA_public_encrypt(flen, (unsigned char *)data.data(), (unsigned char *)out.data(), rsa, RSA_NO_PADDING);
        RSA_private_decrypt(flen, (unsigned char *)out.data(), (unsigned char *)out2.data(), privKey, RSA_NO_PADDING);
    }

    RSA_free(rsa);
    RSA_free(privKey);
}

BOOST_AUTO_TEST_CASE(temp)
{

        using namespace std::chrono;

        const auto systemStart = std::chrono::system_clock::now();

        std::thread th1([&]() {
            test();
        });
        std::thread th2([&]() {
            test();
        });
        std::thread th3([&]() {
            test();
        });
        std::thread th4([&]() {
            test();
        });

        th1.join();
        th2.join();
        th3.join();
        th4.join();

        const auto systemEnd = std::chrono::system_clock::now();
        const auto duration = duration_cast<milliseconds>(systemEnd - systemStart);

        std::cout << duration.count() << "ms" << std::endl;
}

BOOST_AUTO_TEST_SUITE_END()
