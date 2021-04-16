#define BOOST_TEST_MODULE ADSI Encryptor Unit Test
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <cryptopp/base64.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <memory>

#include "codec/DefaultCodecFactory.hpp"
#include "generator/DefaultCryptoKeyGeneratorFactory.hpp"
#include "generator/PublicRSACryptoKeyGenerator.hpp"
#include "utils/DCT.hpp"

BOOST_AUTO_TEST_CASE(dct_algo_test)
{
    try {
        std::vector<std::vector<float>> data {
            { { 52.0f, 55.0f, 61.0f, 66.0f, 70.0f, 61.0f, 64.0f, 73.0f },
              { 63.0f, 59.0f, 55.0f, 90.0f, 109.0f, 85.0f, 69.0f, 72.0f },
              { 62.0f, 59.0f, 68.0f, 113.0f, 144.0f, 104.0f, 66.0f, 73.0f },
              { 63.0f, 58.0f, 71.0f, 122.0f, 154.0f, 106.0f, 70.0f, 69.0f },
              { 67.0f, 61.0f, 68.0f, 104.0f, 126.0f, 88.0f, 68.0f, 70.0f },
              { 79.0f, 65.0f, 60.0f, 70.0f, 77.0f, 68.0f, 58.0f, 75.0f },
              { 85.0f, 71.0f, 64.0f, 59.0f, 55.0f, 61.0f, 65.0f, 83.0f },
              { 87.0f, 79.0f, 69.0f, 68.0f, 65.0f, 76.0f, 78.0f, 94.0f } }
        };

        utils::DCT transform;
        auto result = transform.transfrom(data);
        auto iresult = transform.itransform(result);

        auto [firstResult, secondResult] = std::mismatch(
                iresult.begin(), iresult.end(), data.begin(), [](const auto &lhs, const auto &rhs) {
                    auto [first, second] =
                            std::mismatch(lhs.begin(), lhs.end(), rhs.begin(),
                                          [](const auto &lhs, const auto &rhs) {
                                              return std::roundf(lhs) == std::roundf(rhs);
                                          });
                    return (first == lhs.end()) && (second == rhs.end());
                });
        BOOST_REQUIRE((firstResult == iresult.end()) && (secondResult == data.end()));
    } catch (const std::exception &e) {
        BOOST_FAIL(e.what());
    }
}

BOOST_AUTO_TEST_CASE(sha3_hasher_test)
{
    try {
        constexpr std::string_view text { "A quick brown fox jumps over the lazy dog." };
        constexpr std::string_view preCalHash {
            "16f043d383269059753569d7ebff1a1d88f4d7a8a508d3814ef2e20891cfbd08"
        };
        std::unique_ptr<codec::ICodecFactory> factory {
            std::make_unique<codec::DefaultCodecFactory>()
        };
        std::unique_ptr<codec::ICodec> encoder { factory->createDefaultHashEncoder(text) };

        encoder->execute();
        auto &result = encoder->getCodecResult();
        std::string hashText;
        static_cast<void>(CryptoPP::ArraySource {
                reinterpret_cast<const CryptoPP::byte *>(result.data()), result.size(), true,
                new CryptoPP::HexEncoder { new CryptoPP::StringSink { hashText }, false } });
        encoder = nullptr;
        BOOST_REQUIRE(hashText == preCalHash);
    } catch (const std::exception &e) {
        BOOST_FAIL(e.what());
    }
}

BOOST_AUTO_TEST_CASE(aes_keygen_test)
{
    constexpr std::string_view password { "test" };
    constexpr std::string_view preCalHash {
        "f220cf02e8e0bf291a08d1bfe53e989dae21f2bf20d919cf8a80bbc866c98c62"
    };

    std::unique_ptr<key_generator::ICryptoKeyGeneratorFactory> keyFactory {
        std::make_unique<key_generator::DefaultCryptoKeyGeneratorFactory>()
    };
    std::unique_ptr<codec::ICodecFactory> factory {
        std::make_unique<codec::DefaultCodecFactory>()
    };
    std::unique_ptr<key_generator::ICryptoKeyGenerator> keyGen {
        keyFactory->createDefaultSymEncryptionKey({ password.data(), password.size() })
    };
    
    keyGen->generate();
    auto &aesKey = keyGen->getGeneratedKey();

    std::unique_ptr<codec::ICodec> shaEnc { factory->createDefaultHashEncoder(aesKey) };
    shaEnc->execute();

    auto &key = shaEnc->getCodecResult();
    std::string keyHash;
    static_cast<void>(CryptoPP::ArraySource {
            reinterpret_cast<const CryptoPP::byte *>(key.data()), key.size(), true,
            new CryptoPP::HexEncoder { new CryptoPP::StringSink { keyHash }, false } });
    BOOST_REQUIRE(keyHash == preCalHash);
}

BOOST_AUTO_TEST_CASE(aes_codec_test)
{
    try {
        constexpr std::string_view data { "A quick brown fox jumps over the lazy dog." };
        constexpr std::string_view password { "ADamnSuperStrongPassword" };
        std::unique_ptr<codec::ICodecFactory> factory {
            std::make_unique<codec::DefaultCodecFactory>()
        };
        std::unique_ptr<key_generator::ICryptoKeyGeneratorFactory> keyFactory {
            std::make_unique<key_generator::DefaultCryptoKeyGeneratorFactory>()
        };

        auto keyGen =
                keyFactory->createDefaultSymEncryptionKey({ password.data(), password.size() });
        auto encryptor = factory->createDefaultSymCryptoEncoder(data, keyGen.get());
        encryptor->execute();

        const auto &encResult = encryptor->getCodecResult();
        auto decryptor = factory->createDefaultSymCryptoDecoder(encResult, keyGen.get());
        decryptor->execute();
        const auto &decResult = decryptor->getCodecResult();

        std::string decoded { reinterpret_cast<const char *>(decResult.data()), decResult.size() };
        BOOST_REQUIRE(decoded == data);
    } catch (const std::exception &e) {
        BOOST_FAIL(e.what());
    }
}

BOOST_AUTO_TEST_CASE(rsa_encryption_test)
{
    constexpr std::string_view data { "A quick brown fox jumps over the lazy dog." };
    try {
        CryptoPP::AutoSeededRandomPool rndPool;

        std::unique_ptr<key_generator::ICryptoKeyGeneratorFactory> keyFactory {
            std::make_unique<key_generator::DefaultCryptoKeyGeneratorFactory>()
        };
        auto keyParams = keyFactory->generateASymParams();
        auto prKeyGen = keyFactory->createDefaultPrivateASymEncryptionKey(*keyParams);
        std::string signature;

        std::unique_ptr<codec::ICodecFactory> factory {
            std::make_unique<codec::DefaultCodecFactory>()
        };
        auto signer = factory->createDefaultASymCryptoEncryptor(data, prKeyGen.get());
        signer->execute();
        const auto &rsltSignature = signer->getCodecResult();
        auto begRsltSignature = reinterpret_cast<const char *>(rsltSignature.data());
        signature = std::string { begRsltSignature, begRsltSignature + rsltSignature.size() };

        auto pbKeyGen = keyFactory->createDefaultPublicASymEncryptionKey(*keyParams);
        pbKeyGen->generate();
        CryptoPP::RSASSA_PKCS1v15_SHA_Verifier verifier {
            dynamic_cast<key_generator::PublicRSACryptoKeyGenerator *>(pbKeyGen.get())
                    ->getPublicKey()
        };
        auto success = verifier.VerifyMessage(
                reinterpret_cast<const CryptoPP::byte *>(data.data()), data.length(),
                reinterpret_cast<const CryptoPP::byte *>(signature.c_str()), signature.length());

        BOOST_REQUIRE(success);
    } catch (const std::exception &e) {
        BOOST_FAIL(e.what());
    }
}

BOOST_AUTO_TEST_CASE(base64_encoder_test)
{
    constexpr std::string_view testData { "A quick brownfox jumps over the lazy dog." };
    constexpr std::string_view preCalculated {
        "QSBxdWljayBicm93bmZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZy4="
    };
    std::unique_ptr<codec::ICodecFactory> factory {
        std::make_unique<codec::DefaultCodecFactory>()
    };
    auto codec = factory->createDefaultB2TEncoder(testData);
    codec->execute();
    
    const auto &result = codec->getCodecResult();
    auto begResult = reinterpret_cast<const char *>(result.data());
    std::string resultStr { begResult, begResult + result.size() };
    BOOST_REQUIRE(resultStr == preCalculated);
}

BOOST_AUTO_TEST_CASE(base64_decoder_test)
{
    constexpr std::string_view testData { "A quick brownfox jumps over the lazy dog." };
    constexpr std::string_view preCalculated {
        "QSBxdWljayBicm93bmZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZy4="
    };
    std::unique_ptr<codec::ICodecFactory> factory {
        std::make_unique<codec::DefaultCodecFactory>()
    };
    std::unique_ptr<codec::ICodec> codec { factory->createDefaultB2TDecoder(preCalculated) };
    codec->execute();

    const auto &result = codec->getCodecResult();
    auto begResult = reinterpret_cast<const char *>(result.data());
    std::string resultStr { begResult, begResult + result.size() };
    BOOST_REQUIRE(resultStr == testData);
}