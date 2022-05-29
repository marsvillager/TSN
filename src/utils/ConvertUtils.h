#ifndef _CONVERT_UTILS_H
#define _CONVERT_UTILS_H

#include <string>
#include <vector>

class ConvertUtils {
  public:
    static std::string convertDecToHexString(long long mem, int octs) {
        char* buf = new char(octs * 2);
        sprintf(buf, "%.2X", mem);
        std::string target(buf);
        delete buf;
        return target;
    }

    static std::string converBinToHexString(unsigned char* mem, int len, std::string tok = " ") {
        std::string output = "0x";
        output.append(tok);
        char temp[8];
        for (int i = 0; i < len; ++i) {
            sprintf(temp, "%.2x", mem[i]);
            output.append(temp, 2);
            output.append(tok);
        }
        return output;
    }

    /* conver mac address string to raw mac address */
    static void converMacStringtoBin(const std::string& macString, unsigned char* mac) {
        for (int i = 0, j = 0; i < 6; i++, j += 3) {
            int n = std::stoi(macString.substr(j, 2), 0, 16);
            memcpy(mac + i, (unsigned char*)&n, 1);
        }
    }
};

#endif  // _CONVERT_UTILS_H