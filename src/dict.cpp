#include "dict.h"

std::string append1(const std::string& w) {
    return w + "1";
}

std::string append2(const std::string& w) {
    return w + "2";
}

std::string append3(const std::string& w) {
    return w + "3";
}

std::string leeta(const std::string& w) {
    std::string res = w;
    for (int i = 0; i < w.size(); i++) {
        if (w[i] == 'a')
            res[i] = '4';
    }
    return res;
}

std::string leeto(const std::string& w) {
    std::string res = w;
    for (int i = 0; i < w.size(); i++) {
        if (w[i] == 'o')
            res[i] = '0';
    }
    return res;
}

std::string leete(const std::string& w) {
    std::string res = w;
    for (int i = 0; i < w.size(); i++) {
        if (w[i] == 'e')
            res[i] = '3';
    }

    return res;
}

std::string uppercase(const std::string& w) {
    std::string res = w;
    if (!res.empty()) res[0] = toupper(res[0]);
    return res;
}

std::string capitalize(const std::string& w) {
    std::string res = w;
    for (auto& c : res)
        c = toupper(c);
    return res;
}

std::string appendexcl(const std::string& w) {
    return w + "!";
}