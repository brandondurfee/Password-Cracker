#include "attack.h"
#include "utils.h"

#include <cstring>
#include <openssl/md5.h>

/***
* constructor to create a Cracker class
*/
Cracker::Cracker(struct Config& cfg) {
    this->cfg = cfg;

    // total number of different problem spaces given charset size and length of password (charset_size^length)
    unsigned long long total = 1;
    for (int i = 0; i < cfg.length; i++)
        total *= cfg.charset.length();

    this->total = total;
}

/**
* use a unique index to create a password
*/
void Cracker::indexToPassword(int idx, unsigned char* out, int length) {
    std::string charset = cfg.charset;
    int base = charset.length();

    for (int i = length - 1; i >= 0; --i) {
        out[i] = charset[idx % base];
        idx /= base;
    }
}

/*
* 
*/
struct CrackResult Cracker::crackPassword() {
    struct CrackResult result;
    if (cfg.mode == "brute") {
        if (cfg.use_gpu) {

        } else {
            result = crack_cpu_brute();
        }
    } else {
        
    }
    
    return result;
}


struct CrackResult Cracker::crack_cpu_brute() {
    unsigned char buf[cfg.length + 1];
    buf[cfg.length] = '\0';

    unsigned char digest[MD5_DIGEST_LENGTH];
    unsigned char target_digest[MD5_DIGEST_LENGTH];

    hex_to_bytes(cfg.target_digest, target_digest);

    struct CrackResult result;

    for (int i = 0; i < total; i++) {
        indexToPassword(i, buf, cfg.length);

        MD5(buf, cfg.length, digest);

        if (memcmp(digest, target_digest, MD5_DIGEST_LENGTH) == 0) {
            memcpy(result.digest, digest, MD5_DIGEST_LENGTH);
            result.plaintext = std::string((char*) buf);
            return result;
        }
    }

    result.plaintext = "not found";
    return result;
}