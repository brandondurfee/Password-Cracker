#include "utils.h"

std::string pprint_digest(unsigned char* digest) {
    std::ostringstream oss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        oss << std::hex << std::setw(2) << std::setfill('0')
            << (int)digest[i];
    }

    return oss.str();
}

void hex_to_bytes(const std::string& hex, unsigned char* out) {
    for (int i = 0; i < 16; i++) {
        std::string byte = hex.substr(2 * i, 2);
        out[i] = (unsigned char) strtol(byte.c_str(), nullptr, 16);
    }
}