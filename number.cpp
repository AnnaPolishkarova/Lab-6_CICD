#include "number.h"
#include <string>

void DivideStrByTwo(std::string& str) {
    int add = 0;
    for (int i = 0; i < str.size(); ++i) {
        int number = str[i] - '0' + add * 10;
        add = number % 2;
        str[i] = number / 2 + '0';
    }
}

void MultyplyStrByTwo(std::string& str) {
    int add = 0;
    for (int i = 0; i < str.size(); ++i) {
        int number = (str[i] - '0') * 2 + add;
        add = number / 10;
        str[i] = number % 10 + '0';
    }
    if (add != 0) {
        str += add + '0';
    }
}

void AddOneToStr(std::string& str) {
    int add = 1;
    for (int i = 0; i < str.size() && add != 0; ++i) {
        int number = (str[i] - '0') + add;
        add = number / 10;
        str[i] = number % 10 + '0';
    }
    if (add != 0) {
        str += add + '0';
    }
}

bool strEqualZero(const std::string& str) {
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] != '0') {
            return false;
        }
    }
    return true;
}

uint2022_t from_uint(uint32_t i) {
    uint2022_t result;
    for (int j = 63; j > 0; j--) {
        result.arr[j] = 0;
    }
    result.arr[0] = i;
    return result;
}

uint2022_t from_string(const char* buff) {
    std::string buffer(buff);
    uint2022_t result;
    for (int i = 0; i < 64; i++) {
        result.arr[i] = 0;
    }
    int i = 0;
    while (!strEqualZero(buffer)) {
        if ((buffer.back() - '0') % 2 != 0) {
            result.arr[i / 32] |= (uint32_t(1) << (i % 32));
        }
        ++i;
        DivideStrByTwo(buffer);
    }
    return result;
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t result;
    uint64_t buffer = 0;
    for (int j = 63; j >= 0; j--) {
        result.arr[j] = 0;
    }
    for (int i = 0; i < 64; i++) {
        buffer += lhs.arr[i];
        buffer += rhs.arr[i];
        result.arr[i] = buffer;
        buffer >>= 32;
    }
    return result;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t result;
    for (int j = 63; j >= 0; j--) {
        result.arr[j] = lhs.arr[j];
    }
    for (int i = 0; i < 64; i++) {
        if (result.arr[i] >= rhs.arr[i]) {
            result.arr[i] -= rhs.arr[i];
        }
        else {
            int j = 1;
            while (result.arr[i + j] == uint32_t(0)) {
                result.arr[i + j] = ~result.arr[i + j];
                ++j;
            }
            --result.arr[i + j];
            uint32_t buffer = ~uint32_t(0);
            buffer -= rhs.arr[i];
            buffer += result.arr[i] + 1;
            result.arr[i] = buffer;
        }
    }
    return result;
}

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t result;
    for (int i = 0; i < 64; i++) {
        result.arr[i] = 0;
    }
    for (int i = 0; i < 2022; ++i) {
        if (((lhs.arr[i / 32] >> (i % 32)) & uint32_t(1)) != 0) {
            uint2022_t sub_result;
            for (int i = 0; i < 64; i++) {
                sub_result.arr[i] = 0;
            }
            for (int j = 0; j + i < 2022; ++j) {
                uint32_t i_bit = ((rhs.arr[j / 32] >> (j % 32)) & uint32_t(1));
                if (i_bit != 0) {
                    sub_result.arr[(i + j) / 32] |= (i_bit << ((i + j) % 32));
                }
            }
            result = result + sub_result;
        }
    }
    return result;
}

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs) {
    return uint2022_t();
}

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs) {
    for (int i = 0; i < 64; ++i) {
        if (lhs.arr[i] != rhs.arr[i]) {
            return false;
        }
    }
}

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {
    std::string result = "0";
    for (int i = 2021; i >= 0; --i) {
        MultyplyStrByTwo(result);
        if (((value.arr[i / 32] >> (i % 32)) & uint32_t(1)) != 0) {
            AddOneToStr(result);
        }
    }
    std::reverse(result.begin(), result.end());
    stream << result;

    return stream;
}