#include "attack.h"
#include <iostream>
#include <cmath>

Cracker::Cracker(struct Config& cfg) {
    this->cfg = cfg;
}

void Cracker::indexToPassword(int idx, char* out, int length) {
    std::string charset = cfg.charset;
    int base = charset.length();

    for (int i = length - 1; i >= 0; --i) {
        out[i] = charset[idx % base];
        idx /= base;
    }
}

struct CrackResult Cracker::crackPassword() {
    return crack_cpu_brute();
}

bool bufcmp(char* buf1, char* buf2, int length) {
    for (int i = 0; i < length; i++) {
        if (buf1[i] != buf2[i])
            return false;
    }

    return true;
}

struct CrackResult Cracker::crack_cpu_brute() {
    // std::cout << "hello world\n";
    int total = std::pow(cfg.charset.length(), cfg.length);
    char buf[4];
    char golden[4] = {'t', 'w', 'i', 'n'};
    for (int i = 0; i < total; i++) {
        indexToPassword(i, buf, 4);
        if (bufcmp(buf, golden, 4)) {
            std::cout << "FOUND IT\n";
            break;
        }
    }
    struct CrackResult res;
    res.plaintext = buf;
    res.hash = "hashme";

    return res;
}