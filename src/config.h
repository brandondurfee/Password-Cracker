#pragma once

#include <string>


enum class CrackerType {brute, dict};
enum class DeviceType {cpu, gpu};

struct Config {
    std::string mode;
    int threads = 1;
    int length = 4;
    bool use_gpu = false;
    std::string wordlist;
    std::string target;
    std::string charset = "abcdefghijklmnopqrstuvwxyz";
};

struct CrackResult {
    std::string plaintext;
    std::string hash;
};