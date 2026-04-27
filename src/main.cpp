#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <chrono>

#include "config.h"
#include "attack.h"
#include "utils.h"

// foward declarations
struct Config parse_args(int argc, char* argv[]);
void validate_config(const Config& cfg);
Metrics run_and_measure(Config cfg);

int main (int argc, char* argv[]) {
    Config cfg = parse_args(argc, argv);
    validate_config(cfg);

    std::cout << "Mode: " << cfg.mode << "\n";
    std::cout << "Threads: " << cfg.threads << "\n";
    std::cout << "Length: " << cfg.length << "\n";
    std::cout << "GPU: " << (cfg.use_gpu ? "yes" : "no") << "\n";

    if (cfg.mode == "dict") {
        std::cout << "Wordlist: " << cfg.wordlist << "\n";
        std::cout << "Rules " << cfg.rules << "\n"; 
    }

    std::cout << "Target Digest: " << cfg.target_digest << std::endl;

    run_and_measure(cfg);
    return 0;
}

Metrics run_and_measure(Config cfg) {
    // create a password cracker with the config specified by the user
    Cracker cracker(cfg);

    // start timing
    auto start = std::chrono::high_resolution_clock::now();

    auto res = cracker.crackPassword();

    auto end = std::chrono::high_resolution_clock::now();
    double seconds = std::chrono::duration<double>(end - start).count();

    Metrics m;
    m.seconds = seconds;
    m.hashes_per_sec = cracker.getTotal() / seconds;

    // print
    std::cout << std::endl << "RESULTS:" << std::endl;
    std::cout << "\tdiscovered password: " << res.plaintext << std::endl; 
    std::cout << "\tdiscovered hash: " << pprint_digest(res.digest) << std::endl;
    std::cout << "\tseconds to crack: " << m.seconds << std::endl;
    std::cout << "\t(ignore if early exit) hash/sec: " << m.hashes_per_sec << std::endl;
    return m;
}

struct Config parse_args(int argc, char* argv[]) {
    Config cfg;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "--mode") {
            cfg.mode = argv[++i];
        } else if (arg == "--threads") {
            cfg.threads = std::stoi(argv[++i]);
        } else if (arg == "--gpu") {
            cfg.use_gpu = true;
        } else if (arg == "--wordlist") {
            cfg.wordlist = argv[++i];
        } else if (arg == "--length") {
            cfg.length = std::stoi(argv[++i]);
        } else if (arg == "--charset") {
            cfg.charset = argv[++i];
        } else if (arg == "--target_digest") {
            cfg.target_digest = argv[++i];
        } else if (arg == "--rules") {
            cfg.rules = argv[++i];
        } else if (arg == "--target_digest_file") {
            cfg.target_digest_file = argv[++i];
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            exit(1);
        }
    }

    return cfg;
}

void validate_config(const Config& cfg) {
    if (cfg.mode != "brute" && cfg.mode != "dict") {
        std::cerr << "Error: --mode must be 'brute' or 'dict'\n";
        exit(1);
    }

    if (cfg.mode == "brute" && cfg.length <= 0) {
        std::cerr << "Error: --length must be greater than 0\n";
        exit(1);
    }

    if (cfg.mode == "dict" && cfg.wordlist.empty()) {
        std::cerr << "Error: --wordlist required for dict mode\n";
        exit(1);
    }

    if (cfg.target_digest == "") {
        std::cerr << "Error: target_digest must be a non-null string\n";
        exit(1);
    }

    if (cfg.target_digest.size() != MD5_DIGEST_LENGTH * 2) {
        std::cerr << "Error: please ensure target_digest is a valid 16 Byte md5 digest\n";
        exit(1);
    }

    if (cfg.threads <= 0) {
        std::cerr << "Error: threads must be > 0\n";
        exit(1);
    }

    if (cfg.rules != "none" && cfg.rules != "basic" && cfg.rules != "advanced") {
        std::cerr << "Error: rules must be 'none', 'basic', or 'advanced'\n";
        exit(1);
    }
}