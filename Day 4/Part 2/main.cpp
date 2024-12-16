#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <cstring>

class MD5 {
public:
    MD5();
    void update(const uint8_t* input, size_t length);
    void update(const std::string& input);
    std::string final();
    static std::string hash(const std::string& input);

private:
    void transform(const uint8_t block[64]);
    void encode(uint8_t* output, const uint32_t* input, size_t length);
    void decode(uint32_t* output, const uint8_t* input, size_t length);

    static constexpr uint32_t s[64] = {
        7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
        5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
        4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
        6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
    };
    static constexpr uint32_t K[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x2441453,  0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x4881d05,  0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

    uint8_t buffer[64];
    uint32_t state[4];
    uint64_t bitCount;
    bool finalized;
};

MD5::MD5() : bitCount(0), finalized(false) {
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;
    std::memset(buffer, 0, sizeof(buffer));
}

void MD5::update(const uint8_t* input, size_t length) {
    size_t bufferIndex = bitCount / 8 % 64;
    bitCount += length * 8;

    size_t remaining = 64 - bufferIndex;
    if (length >= remaining) {
        std::memcpy(buffer + bufferIndex, input, remaining);
        transform(buffer);
        input += remaining;
        length -= remaining;
        while (length >= 64) {
            transform(input);
            input += 64;
            length -= 64;
        }
        bufferIndex = 0;
    }
    std::memcpy(buffer + bufferIndex, input, length);
}

void MD5::update(const std::string& input) {
    update(reinterpret_cast<const uint8_t*>(input.data()), input.size());
}

std::string MD5::final() {
    if (finalized) return "";

    uint8_t padding[64] = { 0x80 };
    uint8_t lengthBytes[8];
    encode(lengthBytes, reinterpret_cast<const uint32_t*>(&bitCount), 8);

    size_t bufferIndex = bitCount / 8 % 64;
    size_t padLength = (bufferIndex < 56) ? (56 - bufferIndex) : (120 - bufferIndex);
    update(padding, padLength);
    update(lengthBytes, 8);

    uint8_t digest[16];
    encode(digest, state, 16);
    finalized = true;

    std::ostringstream oss;
    for (size_t i = 0; i < 16; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);

    return oss.str();
}

void MD5::transform(const uint8_t block[64]) {
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3], m[16];

    decode(m, block, 64);
    for (int i = 0; i < 64; ++i) {
        uint32_t f, g;
        if (i < 16) {
            f = (b & c) | (~b & d);
            g = i;
        } else if (i < 32) {
            f = (d & b) | (~d & c);
            g = (5 * i + 1) % 16;
        } else if (i < 48) {
            f = b ^ c ^ d;
            g = (3 * i + 5) % 16;
        } else {
            f = c ^ (b | ~d);
            g = (7 * i) % 16;
        }

        f += a + K[i] + m[g];
        a = d;
        d = c;
        c = b;
        b += (f << s[i]) | (f >> (32 - s[i]));
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}

void MD5::encode(uint8_t* output, const uint32_t* input, size_t length) {
    for (size_t i = 0; i < length / 4; ++i) {
        output[i * 4] = input[i] & 0xff;
        output[i * 4 + 1] = (input[i] >> 8) & 0xff;
        output[i * 4 + 2] = (input[i] >> 16) & 0xff;
        output[i * 4 + 3] = (input[i] >> 24) & 0xff;
    }
}

void MD5::decode(uint32_t* output, const uint8_t* input, size_t length) {
    for (size_t i = 0; i < length / 4; ++i) {
        output[i] = input[i * 4] | (input[i * 4 + 1] << 8) |
                    (input[i * 4 + 2] << 16) | (input[i * 4 + 3] << 24);
    }
}

std::string MD5::hash(const std::string& input) {
    MD5 md5;
    md5.update(input);
    return md5.final();
}

int main() {
    std::string input = "iwrupvqb";
    int number = 0;
    std::string hash;
    std::string code;


    while(true) {
        code = input + std::to_string(number);
        std::string hash = MD5::hash(code);
        if(hash.substr(0, 6) == "000000") {
            std::cout << "Lowest number: " << number << std::endl;
            break;
        }
        number++;
    }
    
    return 0;
}
