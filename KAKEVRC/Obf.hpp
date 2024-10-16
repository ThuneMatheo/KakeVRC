#pragma once
#include <array>
#include <string>
#include <iterator>
#include <algorithm>

namespace Obf {

    inline std::string xor_(std::string s, char key) {
        std::string out{};
        for (char& c : s) {
            out += c ^ key;
        }
        return out;
    }

    template<std::size_t S>
    struct Xor_String {
        std::array<char, S> charArr;
        char key;

        inline auto operator()() {
            std::string str{};
            std::transform(charArr.begin(), charArr.end() - 1, std::back_inserter(str), [this](auto const& c) {
                return c ^ key;  // Use the last character of the original string as the key
                });
            return str;
        }

        constexpr Xor_String(const char(&string)[S]) : charArr{}, key(string[S - 2]) {
            auto it = charArr.begin();
            for (auto const& c : string) {
                *it = c ^ key;  // Encrypt using the last character as the key
                it++;
            }
        }
    };
}

#define obf(string) []() { static auto result = Obf::Xor_String<sizeof(string)/sizeof(char)>(string); return result(); }()