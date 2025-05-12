#include "regex_wrapper.h"
#include "../ctre/ctre.hpp"

namespace NCParser {

regex_wrapper::regex_wrapper() {}

bool regex_wrapper::matchVariable(RegexToken token, std::string text)
{
    switch (token){
    case Token_OSPP300L_1: {
        static constexpr auto pattern = ctll::fixed_string{
            "^V(?:[0-1]?[0-9]?[0-9](?=[^0-9]))$|^V(?:200)$"};
        return ctre::match<pattern>(text);
    }
    case Token_OSPP300L_2: {
        static constexpr auto pattern = ctll::fixed_string{
            "^(?!.*(1[AB]|2[AB]|A[ABNT]|B[ACRT]|C[BCDFLPQR]|D[AB]|F[ABW]|H[AIJKPS]|I[AB]|K[ABW]|"
            "L[ABDK]|M[GNT]|O[FGS]|P[AWXZ]|QA|R[ABCHJTW]|S[ABCP]|T[ABCDEGLMNPRSW]|U[AB]|W[ABW]|X"
            "[ABDEFS]|Z[ABDEF]|SYNC|TF[ABCPQR]|CHD|CHD))(?:[^ONV][A-Z][A-Z0-9][A-Z0-9])$"};
        return ctre::match<pattern>(text);
    }
    case Token_TX8H_1: {
        static constexpr auto pattern = ctll::fixed_string{"^L(?:[0-4]?[0-9])$|^(50)$"};
        return ctre::match<pattern>(text);
    }
    case Token_TX8H_2: {
        static constexpr auto pattern = ctll::fixed_string{"^L1(?:[0-4][0-9])$|^(?:50)$"};
        return ctre::match<pattern>(text);
    }
    }
    return false;
}

}
