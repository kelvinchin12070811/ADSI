#define BOOST_TEST_MODULE ADSI Encryptor Unit Test
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <cryptopp/base64.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <memory>

#include "codec/AESDecoderCodec.hpp"
#include "codec/AESEncoderCodec.hpp"
#include "codec/SHA3EncoderCodec.hpp"
#include "generator/AESCryptoKeyGenerator.hpp"
#include "generator/PrivateRSACryptoKeyGenerator.hpp"
#include "generator/PublicRSACryptoKeyGenerator.hpp"
#include "utils/DCT.hpp"

BOOST_AUTO_TEST_CASE(dct_algo_test)
{
    try
    {
        std::vector<std::vector<float>> data{ {
            { 52.0f, 55.0f, 61.0f, 66.0f, 70.0f, 61.0f, 64.0f, 73.0f },
            { 63.0f, 59.0f, 55.0f, 90.0f, 109.0f, 85.0f, 69.0f, 72.0f },
            { 62.0f, 59.0f, 68.0f, 113.0f, 144.0f, 104.0f, 66.0f, 73.0f },
            { 63.0f, 58.0f, 71.0f, 122.0f, 154.0f, 106.0f, 70.0f, 69.0f },
            { 67.0f, 61.0f, 68.0f, 104.0f, 126.0f, 88.0f, 68.0f, 70.0f },
            { 79.0f, 65.0f, 60.0f, 70.0f, 77.0f, 68.0f, 58.0f, 75.0f },
            { 85.0f, 71.0f, 64.0f, 59.0f, 55.0f, 61.0f, 65.0f, 83.0f },
            { 87.0f, 79.0f, 69.0f, 68.0f, 65.0f, 76.0f, 78.0f, 94.0f }
        } };

        utils::DCT transform;
        auto result = transform.transfrom(data);
        auto iresult = transform.itransform(result);

        auto [firstResult, secondResult] = std::mismatch(
            iresult.begin(),
            iresult.end(),
            data.begin(),
            [](const auto &lhs, const auto &rhs) {
                auto [first, second] = std::mismatch(
                    lhs.begin(),
                    lhs.end(),
                    rhs.begin(),
                    [](const auto &lhs, const auto &rhs) { return std::roundf(lhs) == std::roundf(rhs); }
                );
                return (first == lhs.end()) && (second == rhs.end());
            }
        );
        BOOST_REQUIRE((firstResult == iresult.end()) && (secondResult == data.end()));
    }
    catch (const std::exception &e)
    {
        BOOST_FAIL(e.what());
    }
}

BOOST_AUTO_TEST_CASE(sha3_hasher_test)
{
    try
    {
        constexpr std::string_view text{ "A quick brown fox jumps over the lazy dog." };
        constexpr std::string_view preCalHash{ "16f043d383269059753569d7ebff1a1d88f4d7a8a508d3814ef2e20891cfbd08" };
        auto encoder = std::make_unique<codec::SHA3EncoderCodec>(text);


        encoder->execute();
        auto result = encoder->getCodecResult();
        std::string hashText;
        static_cast<void>(CryptoPP::ArraySource{
            reinterpret_cast<CryptoPP::byte *>(result.data()),
            result.size(),
            true,
            new CryptoPP::HexEncoder{
                new CryptoPP::StringSink{ hashText },
                false
            }
        });
        encoder = nullptr;
        BOOST_REQUIRE(hashText == preCalHash);
    }
    catch (const std::exception &e)
    {
        BOOST_FAIL(e.what());
    }
}

BOOST_AUTO_TEST_CASE(aes_keygen_test)
{
    constexpr std::string_view password{ "test" };
    constexpr std::string_view preCalHash{ "f220cf02e8e0bf291a08d1bfe53e989dae21f2bf20d919cf8a80bbc866c98c62" };

    key_generator::AESCryptoKeyGenerator keyGen{ { password.data(), password.length() } };
    keyGen.generate();
    auto aesKey = keyGen.getGeneratedKey();

    codec::SHA3EncoderCodec shaEnc;
    shaEnc.setCodecData(aesKey);
    shaEnc.execute();
    
    auto key = shaEnc.getCodecResult();
    std::string keyHash;
    static_cast<void>(CryptoPP::ArraySource{
        reinterpret_cast<CryptoPP::byte *>(key.data()),
        key.size(),
        true,
        new CryptoPP::HexEncoder{
            new CryptoPP::StringSink{ keyHash },
            false
        }
    });
    BOOST_REQUIRE(keyHash == preCalHash);
}

BOOST_AUTO_TEST_CASE(aes_codec_test)
{
    try
    {
        constexpr std::string_view data{ "A quick brown fox jumps over the lazy dog." };
        constexpr std::string_view password{ "ADamnSuperStrongPassword" };
        std::vector<std::byte> AESKey;

        key_generator::AESCryptoKeyGenerator keyGen{ { password.data(), password.size() } };
        keyGen.generate();
        AESKey = keyGen.getGeneratedKey();

        codec::AESEncoderCodec encryptor{ data, AESKey };
        encryptor.execute();
        auto encResult = encryptor.getCodecResult();

        codec::AESDecoderCodec decryptor{ encResult, AESKey };
        decryptor.execute();
        auto decResult = decryptor.getCodecResult();

        std::string decoded{ reinterpret_cast<char *>(decResult.data()), decResult.size() };
        BOOST_REQUIRE(decoded == data);
    }
    catch (const std::exception &e)
    {
        BOOST_FAIL(e.what());
    }
}

BOOST_AUTO_TEST_CASE(rsa_encryption_test)
{
    constexpr std::string_view data{ "A quick brown fox jumps over the lazy dog." };
    try
    {
        auto keyParams = key_generator::RSACryptoKeyGeneratorBase::generateKeyParams();
        key_generator::PrivateRSACryptoKeyGenerator engPrivate{ keyParams };
        key_generator::PublicRSACryptoKeyGenerator engPublic{ keyParams };
        
        CryptoPP::AutoSeededRandomPool rndPool;

        engPrivate.generate();
        engPublic.generate();
        auto privateKey = engPrivate.getPrivatekey();
        auto publicKey = engPublic.getPublicKey();

        CryptoPP::RSAES_OAEP_SHA_Encryptor encoder{ publicKey };
        std::string datEncoded;
        static_cast<void>(CryptoPP::StringSource{
            { data.data(), data.size() },
            true,
            new CryptoPP::PK_EncryptorFilter{
                rndPool,
                encoder,
                new CryptoPP::Base64Encoder{
                    new CryptoPP::StringSink{ datEncoded },
                    false
                }
            }
        });

        CryptoPP::RSAES_OAEP_SHA_Decryptor decryptor{ privateKey };
        std::string datDecoded;
        static_cast<void>(CryptoPP::StringSource{
            datEncoded,
            true,
            new CryptoPP::Base64Decoder{
                new CryptoPP::PK_DecryptorFilter{
                    rndPool,
                    decryptor,
                    new CryptoPP::StringSink{ datDecoded }
                }
            }
        });

        BOOST_REQUIRE(datDecoded == data);
    }
    catch (const std::exception &e)
    {
        BOOST_FAIL(e.what());
    }
}