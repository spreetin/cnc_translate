#ifndef TOKENS_H
#define TOKENS_H

/*
 * This file defines tokens output during lexing, as well as differnt
 * parameters used to define the function of differnt parts of code.
 */

#include <string>
#include <vector>

namespace NCParser {

    namespace Token {
        // Character literal tokens
        static const int newline = '\n';
        static const int percent = '%';
        static const int leftparen = '(';
        static const int rightparen = ')';
        static const int star = '*';
        static const int plus = '+';
        static const int comma = ',';
        static const int minus = '-';
        static const int period = '.';
        static const int slash = '/';
        static const int colon = ':';
        static const int equals = '=';
        static const int leftsquare = '[';
        static const int rightsquare = ']';
        static const int bang = '!';
        static const int semicolon = ';';
        static const int dollar = '$';

        // NC words
        static const int a_word = 'A';
        static const int b_word = 'B';
        static const int c_word = 'C';
        static const int d_word = 'D';
        static const int e_word = 'E';
        static const int f_word = 'F';
        static const int g_word = 'G';
        static const int h_word = 'H';
        static const int i_word = 'I';
        static const int j_word = 'J';
        static const int k_word = 'K';
        static const int l_word = 'L';
        static const int m_word = 'M';
        static const int n_word = 'N';
        static const int o_word = 'O';
        static const int p_word = 'P';
        static const int q_word = 'Q';
        static const int r_word = 'R';
        static const int s_word = 'S';
        static const int t_word = 'T';
        static const int u_word = 'U';
        static const int v_word = 'V';
        static const int w_word = 'W';
        static const int x_word = 'X';
        static const int y_word = 'Y';
        static const int z_word = 'Z';

        // Data types
        static const int num_literal = 200;
        static const int num_int = 201;
        static const int num_float = 202;
        static const int string = 203;

        // Abstract functions
        static const int subsystem_select = 245;
        static const int block_delete = 246;
        static const int queueing_num = 247;
        static const int queueing = 248;
        static const int unknown_function = 249;
        static const int left_over_data = 250;
        static const int unary_function = 251;
        static const int binary_functions = 252;
        static const int multary_functions = 253;
        static const int block_function = 254;
        static const int parameter = 255;
        static const int multi_letter = 256;
        static const int prg_name = 257;
        static const int variable = 258;
        static const int comment = 259;
        static const int unknown_code = 260;
        static const int error = 261;
        static const int done = 262;
        static const int block = 263;
        static const int list = 264;
    };

    enum Duration {
        DDFC,
        FRC,
        FRC_A,
        FRC_B,
        FRC_C,
        FRC_D,
        FRC_E,
        FRC_F,
        FRC_I,
        FRC_G,
        FRC_K,
        FRC_M,
        TBO,
        None,
    };

    enum ActionScope {
        AAM,
        AWM,
        AAM_AWM,
        UndefinedScope,
    };

    struct FixedCycleDefinition {
        enum movement {
            FeedIn,
            Intermittent,
            Forward_Spindle_Feed,
            Start_Spindle_Feed,
        } Movement;
        enum bottom_dwell {
            Yes,
            No
        } BottomDwell;
        enum bottom_spindle {
            Reverse,
            Stop,
            None
        } BottomSpindle;
        enum movement_out {
            Rapid,
            FeedOut,
            Manual,
        } MovementOut;
        bool valid = false;
    };

    struct GCode {
        std::string Name;
        std::vector<char> dimensions;
        int function;
        Duration duration;
    };

    struct MCode {
        std::string Name;
        int function;
        Duration duration;
        ActionScope scope;
    };

};

#endif // TOKENS_H
