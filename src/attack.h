// class defines the attack functions that will be used by attack.cpp

#pragma once

#include <string>
#include "config.h"

class Cracker {
public:
    Cracker(struct Config& cfg);
    
    // crack the password based on the config
    struct CrackResult crackPassword();
private:
    struct Config cfg;
    std::string hash;
    unsigned long long total;

    /* different ways to crack the password called by crackPassword */
    // 1. crack with CPU brute force
    struct CrackResult crack_cpu_brute();

    // 2. crack with CPU using dictionary attack
    struct CrackResult crack_cpu_dict();

    // 3. crack with GPU using brute force
    struct CrackResult crack_gpu_brute();

    // function to turn the index into a generated password
    void indexToPassword(int index, unsigned char* buf, int length);
};



