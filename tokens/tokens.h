#ifndef TOKENS_H
#define TOKENS_H

/*
 * This file defines tokens output during lexing, as well as differnt
 * parameters used to define the function of differnt parts of code.
 */

#include <string>
#include <vector>

namespace NCParser {
    /*
     * Implementation agnostic G- and M-tokens. Programs will first be converted to a
     * series of such tokens, before processing, as to make parsing more general.
     *
     * Adding new machine functions not available in ISO 6983-1:2009 also means
     * that the corresponding abstract tokens needs to be added here.
     */

    // Prepatory (G-code) functions
    namespace GCodes {
        static const int rapid_positioning = 0;
        static const int linear_interpolation = 1;
        static const int circular_interpolation_cw = 2;
        static const int circular_interpolation_ccw = 3;
        static const int dwell = 4;
        static const int parabolic_interpolation = 6;
        static const int exact_stop = 9;
        static const int xy_plane_selection = 17;
        static const int zx_plane_selection = 18;
        static const int zy_plane_selection = 19;
        static const int thread_cutting_constant_lead = 33;
        static const int thread_cutting_increasing_lead = 34;
        static const int thread_cutting_decreasing_lead = 35;
        static const int cutter_compensation_cancel = 40;
        static const int cutter_compensation_left = 41;
        static const int cutter_compensation_right = 42;
        static const int tool_offset_positive = 43;
        static const int tool_offset_negative = 44;
        static const int dimension_shift_cancel = 53;
        static const int zero_shift = 54;
        static const int tapping = 63;
        static const int continuous_path_mode = 64;
        static const int dimension_input_inch = 70;
        static const int dimension_input_metric = 71;
        static const int home_position = 74;
        static const int fixed_cycle_cancel = 80;
        static const int fixed_cycle_81 = 81;
        static const int fixed_cycle_82 = 82;
        static const int fixed_cycle_83 = 83;
        static const int fixed_cycle_84 = 84;
        static const int fixed_cycle_85 = 85;
        static const int fixed_cycle_86 = 86;
        static const int fixed_cycle_87 = 87;
        static const int fixed_cycle_88 = 88;
        static const int fixed_cycle_89 = 89;
        static const int absolute_dimension = 90;
        static const int incremental_dimension = 91;
        static const int preload_registers = 92;
        static const int inverse_time = 93;
        static const int feed_per_minute = 94;
        static const int feed_per_revolution = 95;
        static const int constant_surface_speed = 96;
        static const int revolutions_per_minute = 97;
    };

    // Miscellaneous (M-code) functions
    namespace MCodes {
        static const int program_stop = 0;
        static const int optional_stop = 1;
        static const int end_of_program = 2;
        static const int spindle_cw = 3;
        static const int spindle_ccw = 4;
        static const int spindle_off = 5;
        static const int tool_change = 6;
        static const int m07 = 7;
        static const int m08 = 8;
        static const int m09 = 9;
        static const int clamp_workpiece = 10;
        static const int unclamp_workpiece = 11;
        static const int end_of_data = 30;
        static const int cancel_m49 = 48;
        static const int bypass_override = 49;
        static const int workpiece_change = 60;
    };

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

        // NC words
        static const int a_word = 'A';
        static const int b_word = 'B';
        static const int c_word = 'C';
        static const int d_word = 'D';
        static const int e_word = 'E';
        static const int f_word = 'F';
        static const int g_word = 'G';
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
        static const int num_int = 200;
        static const int num_float = 201;
        static const int string = 202;

        // Abstract functions
        static const int prg_name = 257;
        static const int variable = 258;
        static const int comment = 259;
        static const int unknown_code = 260;
        static const int error = 261;
        static const int done = 262;
        static const int block = 263;
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
