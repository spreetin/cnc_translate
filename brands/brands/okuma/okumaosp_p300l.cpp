#include "okumaosp_p300l.h"

namespace NCParser {

MachineParameters OkumaOSP_P300L::getParameters()
{
    MachineParameters param;
    updateParameters(&param);

    // General settings
    param.general.has_nose_radius_compenstion = true;
    param.maths.use_block_parenthesis = true;
    param.maths.hexadecimal = '$';
    param.queueing.queueing_numbered_single_char = 'P';
    param.queueing.queueing_string = "M10";
    param.subsystem.symbol = 'G';
    param.subsystem.number = 2;

    // Axis
    param.parameter.subtypes['X'] = {1};
    param.parameter.subtypes['Y'] = {1};
    param.parameter.subtypes['Z'] = {1};
    param.parameter.subtypes['C'] = {1};
    param.parameter.is_diameter.insert('X');

    // Define changed default word meanings
    param.parameter.defaults['A'] = param_undefined;
    param.parameter.defaults['B'] = param_undefined;
    param.parameter.defaults['C'] = param_c_axis;
    param.parameter.defaults['D'] = param_undefined;
    param.parameter.defaults['E'] = param_undefined;
    param.parameter.defaults['H'] = param_undefined;
    param.parameter.defaults['L'] = param_circle_radius;
    param.parameter.defaults['P'] = param_queueing_numbered;
    param.parameter.defaults['S'] = param_undefined;
    param.parameter.defaults['O'] = param_prg_name;
    param.parameter.defaults['U'] = param_undefined;
    param.parameter.defaults['W'] = param_undefined;

    param.parameter.defaults.erase('I');
    param.parameter.defaults.erase('J');
    param.parameter.defaults.erase('K');

    param.parameter.defaults_multiletter["QA"] = param_c_axis_rotation;
    param.parameter.defaults_multiletter["SA"] = param_c_axis_rpm;
    param.parameter.defaults_multiletter["SB"] = param_tool_rpm;
    param.parameter.defaults_multiletter["TL"] = param_tool_preparation;
    param.parameter.defaults_multiletter["TD"] = param_tool_preparation;

    param.parameter.always_useable = {'S', 'T'}; // SB, TL, TD, TS

    // Variables
    param.subprograms.available_variables = {Token_OSPP300L_1};
    param.variables.available_variables = {Token_OSPP300L_1, Token_OSPP300L_2};

    // Functions
    param.functions.unary = {{"NOT", f_not_b}, {"SIN", f_sine}, {"COS", f_cosine}, {"TAN", f_tangent},
                             {"ATAN", f_arctangent}, {"SQRT", f_square_root}, {"ABS", f_abs},
                             {"BIN", f_dec_to_bin}, {"BCD", f_bin_to_dec}, {"ROUND", f_round},
                             {"FIX", f_floor}, {"FUP", f_ceil}, {"DROUND", f_round_decimals},
                             {"DFIX", f_floor_decimals}, {"DFUP", f_ceil_decimals}, {"GOTO", f_jump},
                             {"IF", f_if}};
    param.functions.binary = {{"EOR", f_xor_b}, {"OR", f_or_b}, {"AND", f_and_b}, {"LT", f_comparison_lesser},
                              {"LE", f_comparsion_lesser_eq}, {"EQ", f_comparison_equal}, {"NE", f_comparison_not_equal},
                              {"GT", f_comparison_greater}, {"GE", f_comparison_greater_eq}};
    param.functions.coordinate = {{"ATAN2", f_arctangent2}, {"MOD", f_mod}};

    // Tool data
    param.tool_selection.parts = {
        {"T",
            {{param_tool_selection_nrc, 2},
            {param_tool_selection_tool, 2},
             {param_tool_selection_offset, 2}}},
        {"TL",
            {{param_tool_selection_nrc, 3},
            {param_tool_selection_tool, 3}}},
        {"TD",
            {{parma_tool_selection_pos, 2},
            {param_tool_selection_tool, 4}}},
    };

    // Unset default G-codes that are not valid
    param.g[g_dimension_input_inch] = {};
    param.g[g_dimension_input_metric] = {};

    // Set all defined G-codes
    param.g[g_rapid_positioning] = {0, gmode_motion,
            {'X', 'Z', 'C'}};
    param.g[g_linear_interpolation] = {1, gmode_motion,
            {'X', 'Z', 'C', 'F', 'A'},
            {{'A', param_angle}}};
    param.g[g_circular_interpolation_cw] = {2, gmode_motion,
            {'X', 'Z', 'I', 'K', 'Y', 'L', 'F'},
            {{'I', param_circle_center_x},
             {'K', param_circle_center_z}}};
    param.g[g_circular_interpolation_ccw] = {3, gmode_motion,
            {'X', 'Z', 'I', 'K', 'Y', 'L', 'F'},
            {{'I', param_circle_center_x},
             {'K', param_circle_center_z}}};
    param.g[g_dwell] = {4}; // Parameters
    // 10 - Parallell och roterande växling av koordinatsystem FRÅN
    // 11 - Parallell och roterande växling av koordinatsystem TILL
    param.g[g_use_subsystem_a] = {13};
    param.g[g_use_subsystem_b] = {14};
    // 15 - Val av arbterskoordinatsystem (modal)
    // 16 - Val av arbterskoordinatsystem (monostabil)
    param.g[g_xy_plane_selection] = {17, gmode_plane_selection};
    param.g[g_zx_plane_selection] = {18, gmode_plane_selection};
    param.g[g_yz_plane_selection] = {19, gmode_plane_selection};
    param.g[g_home_position] = {20, gmode_motion};
    // 21 - Flytta till ATC utgångsposition
    param.g[g_chuck_transfer_under_torque] = {22, -1,
            {'Z', 'D', 'L', 'F'}}; // Parameter PZ
    // 23 - Kontroll av vridmomentsgräns
    // 24 - ATC förlyttning av positon (ingen interpolation)
    // 25 - Anslut positionsförflyttning (ingen interpolation)
    param.g[g_torque_limit_chuck_transfer_off] = {28};
    param.g[g_torque_limit_chuck_transfer] = {29, -1,
            {'P', 'Z', 'W'}}; // Needs more work
    // 30 - Överhoppscykel
    // 31 - as 33 but lengthwise?
    param.g[g_thread_cutting_end_face] = {32, gmode_motion,
            {'X', 'Z', 'F', 'I', 'A', 'E', 'K', 'L', 'J', 'C'},
            {{'F', param_thread_lead},
             {'I', param_conical_amount},
             {'A', param_conical_angle},
             {'E', param_thread_lead_change},
             {'K', param_thread_start_offset},
             {'L', param_thread_champfer_end_length},
             {'J', param_thread_lead_count}}}; // C (fasskillnad)
    param.g[g_thread_cutting_variable_lead] = {33, gmode_motion,
            {'X', 'Z', 'F', 'I', 'A', 'E', 'K', 'L', 'J', 'C'},
            {{'F', param_thread_lead},
             {'I', param_conical_amount},
             {'A', param_conical_angle},
             {'E', param_thread_lead_change},
             {'K', param_thread_start_offset},
             {'L', param_thread_champfer_end_length},
             {'J', param_thread_lead_count}}}; // C (fasskillnad)
    param.g[g_thread_cutting_multistep] = {33, gmode_motion,
            {'X', 'Z', 'F', 'E', 'J', 'C'},
            {{'F', param_thread_lead},
             {'E', param_thread_lead_change},
             {'J', param_thread_lead_count}}}; // C (fasskillnad)
    // 34 - thread_cutting_cycle_decreasing_lead
    // 35 - thread_cutting_cycle_increasing_lead
    // 36 - Synkroniserad matning vid matningsaxeln för M-verktygets spindel: framåt
    // 37 - Synkroniserad matning via matningsaxeln för M-verktygers spindel: bakåt
    // 39 - Båg-kommando för hörn
    param.g[g_cutter_compensation_cancel] = {40};
    param.g[g_cutter_compensation_left] = {41};
    param.g[g_cutter_compensation_right] = {42};
    // 48 - APA påbörja sökning
    // 49 - APA avsluta sökning
    param.g[g_zero_set] = {50, -1,
            {'X', 'Z', 'C'},
            {{'X', param_set_x_value_on_current},
             {'Z', param_set_z_value_on_current},
             {'C', param_set_c_value_on_current}}};
    param.g[g_rpm_limit] = {50, -1,
            {'S'},
            {{'S', param_rpm_max}}};
    // 51 - Välj arbetskoordinater
    // 52 - Revolver indexposition felkompensation
    // 54 - Verktygslängdskompensering x-axel
    // 55 - Verktygslängdskompensering y-axel
    // 56 - Verktygslängdskompensering z-axel
    // 62 - Bestämma spegelvändning
    param.g[g_servo_error_compensation_off] = {64};
    param.g[g_servo_error_compensation_on] = {65};
    // 69 - Längsgående gängskärningscykler
    // 70 - Tvärgående gängskärningscykler
    param.g[g_thread_cutting_cycle] = {71, gmode_motion,
            {'X', 'Z', 'A', 'I', 'B', 'D', 'U', 'H', 'L', 'E', 'F', 'J', 'M', 'Q'},
            {{'A', param_conical_angle},
             {'I', param_conical_amount},
             {'B', param_angle},
             {'D', param_thread_depth_first_cut},
             {'U', param_margin},
             {'H', param_thread_depth},
             {'L', param_thread_champfer_end_length},
             {'E', param_thread_lead_change},
             {'F', param_thread_lead},
             //{'M', param_thread_champfer_end_length}, Special
             //{'Q', param_thread_champfer_end_length}, Special
             {'J', param_thread_lead_count},
             {'C', param_thread_phase_difference}}};
    param.g[g_thread_cutting_cycle_end_face] = {72, gmode_motion,
            {'X', 'Z', 'A', 'I', 'B', 'D', 'U', 'H', 'L', 'E', 'F', 'J', 'M', 'Q'},
            {{'A', param_conical_angle},
             {'I', param_conical_amount},
             {'B', param_angle},
             {'D', param_thread_depth_first_cut},
             {'U', param_margin},
             {'H', param_thread_depth},
             {'L', param_thread_champfer_end_length},
             {'E', param_thread_lead_change},
             {'F', param_thread_lead},
             //{'M', param_thread_champfer_end_length}, Special
             //{'Q', param_thread_champfer_end_length}, Special
             {'J', param_thread_lead_count},
             {'C', param_thread_phase_difference}}};
    param.g[g_wedge_cut_cycle_lengthwise] = {73, gmode_motion,
            {'X', 'Z', 'I', 'K', 'D', 'L', 'E', 'T'},
            {{'D', param_milling_depth},
             // I - kör i x-axelns riktning
             // K - kör i z-axelns riktning
             {'L', param_milling_depth_tool_removal},
             {'E', param_dwell_time},
             {'T', param_tool_selection_offset}}};
    param.g[g_drill_cycle_z_axis] = {74, gmode_motion,
            {'X', 'Z', 'I', 'K', 'D', 'L', 'E', 'T'},
            {{'D', param_milling_depth},
             // I - kör i x-axelns riktning
             // K - kör i z-axelns riktning
             {'L', param_milling_depth_tool_removal},
             {'E', param_dwell_time},
             {'T', param_tool_selection_offset}}};
    param.g[g_transfer_chamfer] = {75, -1,
            {'L'},
            {{'L', param_transfer_chamfer}}, {}, true};
    param.g[g_transfer_radius] = {76, -1,
            {'L'},
            {{'L', param_transfer_chamfer}}, {}, true};
    param.g[g_thread_cutting_circular_cw] = {77, gmode_motion,
            {'X', 'Z', 'K', 'F'},
            {{'X', param_x_start},
             {'K', param_rapid_feed},
             {'F', param_feed}}};
    param.g[g_thread_cutting_circular_ccw] = {78, gmode_motion,
            {'X', 'Z', 'K', 'F'},
            {{'X', param_x_start},
             {'K', param_rapid_feed},
             {'F', param_feed}}};
    // G80, G81, G82, G83, G84, G85, G86, G87, G88 - automatiska skrubbcykler (LAP)
    param.g[g_absolute_dimension] = {90, gmode_distance};
    param.g[g_incremental_dimension] = {91, gmode_distance};
    // 93 - Inverterad tidsmatning
    param.g[g_feed_per_minute] = {94, gmode_feed_rate,
            {'F'},
            {{'F', param_feed}}};
    param.g[g_feed_per_revolution] = {95, gmode_feed_rate,
            {'F'},
            {{'F', param_feed}}};
    param.g[g_constant_surface_speed] = {96, gmode_spindle_speed,
            {'S'},
            {{'S', param_rpm}}};
    param.g[g_revolutions_per_minute] = {97, gmode_spindle_speed,
            {'S'},
            {{'S', param_rpm}}};
    // 98 - Interferenskontroll och spåneffektivitet
    // 99 - Interferenskontroll och spån ogiltigt
    // 100 - Prioritetskommando för revolver A eller B oberoende skärning
    param.g[g_milling_line_end_face] = {101, gmode_motion,
            {'X', 'Z', 'C', 'F'}};
    param.g[g_milling_curve_cw_end_face] = {102, gmode_motion,
            {'X', 'Z', 'C', 'L', 'F'}};
    param.g[g_milling_curve_ccw_end_face] = {103, gmode_motion,
            {'X', 'Z', 'C', 'L', 'F'}};
    // 104 - x-axel spegling FRÅN
    // 105 - x-axel spegling TILL
    // 107 - Synkroniserad spindelgängning: cw gänga
    // 108 - Synkroniserad spindelgängning: ccw gänga
    // 110 - Konstant skärhastighet revolver A
    // 111 - Konstant skärhastighet revolver B
    // 112 - SKärning av cylindrisk gänga cw
    // 113 - Skärning av cylindrisk gänga ccw
    // 114 - Beteckning av revolver B
    // 115 - Val av arbetskoordinatsystem (10sets, 50sets spec.)
    // 116 - W-Z-axelöverlappningsläga FRÅN
    // 117 - W-Z-axelöverlappningsläga TILL
    // 118 - Sammansatt fast gängskärningscykel: bestämning av slutpunkt
    param.g[g_xz_cylindrical_plane_selection] = {119, gmode_plane_selection};
    // 120 - c-axel multiplanläga FRÅN
    // 121 - c-axel multiplanläga TILL
    // 122 - W-axelkommando för extraspindel på revolver A
    // 123 - W-axelkommando för extraspindel på revolver B
    // 124 - A-chuck val av koordinatsystem (ACC)
    // 125 - B-chuck val av koordinatsystem (ACC)
    // 126 - Lutande bearbetningsmod FRÅN
    // 127 - Lutande bearbetningsmod TILL
    // 128 - Inaktivera maskinläga
    // 129 - Bearbetningsmod
    // 130 - C-axel för identifikation av vinkelfas
    // 131 - Sensor-hopp-kommando
    param.g[g_circular_interpolation_cw_milling_on_turning] = {132, gmode_motion,
            {'Z', 'C', 'L', 'F'}};
    param.g[g_circular_interpolation_ccw_milling_on_turning] = {133, gmode_motion,
            {'Z', 'C', 'L', 'F'}};
    // 134 - Z-axel A-revolver automatisk
    // 135 - Z-axel B-revolver automatisk
    param.g[g_coordinate_cartesian_to_polar_off] = {136};
    param.g[g_coordinate_cartesian_to_polar] = {137, -1,
            {'C'}};
    // 138 - Y-axelläge TILL
    // 139 - Y-axel svarvposition snabb positionering
    // 140 - Bestämma bearbetning med huvudspindel
    // 141 - Bestämma bearbetning med extraspindel
    // 142 - Bestämma bearbetning med dockningsspindel
    // 143 - Tredje revolverläget
    // 144 - Kommando för W-axelkontroll FRÅN
    // 145 - W-axelövervakning TILL
    // 148 - B-axelläge FRÅN
    // 149 - B-axelläge TILL
    // 150 - M/C skärläge inaktiverat
    // 151 - M/C skärläge aktiverat
    // 152 - Programmerbar dubbdocksrörelse
    // 153 - Programmerbar positionering av fast stöddocka
    // 154 - W-axel enkelriktad positionering
    // 155 - Exakt kurvlinjeläge TILL
    // 156 - Exakt kurvlinjeläge FRÅN
    // 157 - G-kodmakrofunktion, slipning aktiverad
    // 158 - Verktygslängdsförskjutning i verktygsaxel riktning
    // 159 - Verktygslängdsförskjutning i verktygsaxel riktning (utan rotationspositionsförskjutning)
    // 160 - Ånga verktygslängdsförskjutning i verktygsaxel riktning
    // 161-170 - G-kod makrofunktion: MODIN
    // 171-176 - G-kod makrofunktion: CALL (fast underprogramnamn)
    param.g[g_fixed_cycle_cancel] = {180};
    // 194 - Initiera villkorsmakro
    // 195 - NC Dubbdocka Flerpositionsläge
    // 196 - FOPENA
    // 197 - FOPENB
    // 198 - FWRITC
    // 199 - CLOSE
    // 200 - Hanteringskommando för verktygsbearbetningsinformation
    // 201 - CASCME (CAS)
    // 202 - CASML
    // 203 - CALRG
    // 204 - CASCTL (CAS)
    // 205-214 - G-kod makrofunktion: CALL
    // 215 - MSG
    // 216 - NMSG
    // 217 - COPY
    // 218 - COPYE
    // 219 - RTMCR
    // 220 - NOEX
    // 221 - CALL
    // 222 - RTS
    // 223 - MODIN
    // 224 - MODOUT
    // 225 - GOTO
    // 226 - IF
    // 227 - ROBOT
    // 228 - PRNT
    // 229 - DEF
    // 230 - DELETE
    // 231 - SAVE
    // 232 - READ
    // 233 - WRITE
    // 234 - DRAW
    // 235 - CLEAR
    // 236 - LCLEAR
    // 237 - MHPS
    // 238 - LOADER
    // 239 - WCPY
    // 240 - TLID
    // 241 - EIN Tillåt avbrott
    // 242 - DIN Neka avbrott
    // 243 - RTI Återuppta avbrott
    // 244 - RSPN (för 2SP)
    // 245 - LSPN (för 2SP)
    // 246 - XCAM (för CAM-svarv)
    // 247 - XCAM1 (för CAM-svarv)
    // 248 - FCALLB (för CAM-svarv)
    // 249 - FCALL (för CAM-svarv)
    // 250 - LMW
    // 251 - LMV
    // 252 - NCYL (fixed cykel ignorera axelrörelse)
    // 253 - RTMDI
    // 254 - Verktygets mittpunktskontrolläge FRÅN
    // 255 - Verktygets mittpunktskontrolläge TILL
    // 256 - Spindeltröghetsindentifikation (MACRO)
    // 263 - Identifikationsfunktion för spindeltröghet
    // 264 - Super-NURBS kontroll FRÅN/Hi-CUT Pro kontroll FRÅN
    // 265 - Super-NURBS kontroll TILL/ Hi-Cut Pro kontroll TILL
    // 267 - Bearbetningsläge (svarvning)
    // 268 - Bearbetningsläge (C-axel svarvning)
    // 269 - Bearbetningsläge (Y-axel svarvning)
    param.g[g_turning_mode] = {270};
    param.g[g_c_axis_mode] = {271};
    param.g[g_y_axis_mode] = {272};
    // 275 - RTMCRP (Koordinatberäkningsfunktionen: Skriv in koordinat Makro)
    // 276 - LOMIT (Koordinatberäkningsfunktionen : utelämna)
    // 277 - RSTRT (Koordinatberäkningsfunktionen : Omstart)
    // 278 - LAA (Koordinatberäkningsfunktionen : linje i vinkel)
    // 279 - GRDX (Koordinatberäkningsfunktionen: Rutnät X)
    // 280 - GRDY (Koordinatberäkningsfunktionen : Rutnät Y)
    // 281 - DGRDX (Koordinatberäkningsfunktionen: houndstooth kontrollera X)
    // 282 - DGRDY (koordinatberäkningsfunktionen : houndstooth kontrollera Y)
    // 283 - SQRX (Koordinatberäkningsfunktionen : Fyrkant X
    // 284 - SQRY (Koordinatberäkningsfunktionen : Fyrkant Y
    // 285 - BHC (koordinatberäkningsfunktionen): bulthålscykel
    // 286 - ARC (Koordinatberäkningsfunktionen) : Båge
    // 287 - MCRNO (koordinatberäkningsfunktionen) : Makro ogiltig
    // 290 - SKRIPT (SKRIPT-Funktionen)
    // 291 - SLOAD (SKRIPT-Funktionen)
    // 292 - SCALL (SKRIPT-Funktionen)
    // 293 - SCALLS (SKRIPT-Funktionen)
    // 294 - SCALLE (SKRIPT-Funktionen)
    // 295 - SCALLSE (SKRIPT-Funktionen)
    // 296 - M-verktygets sammansatta fasta cykel: fin arborrningscykel
    // 297 - M-verktygets sammansatta fasta cykel: bakborrningscykel
    // 298 - M-verktygets sammansatta fasta cykel: gängningscykel bakåt
    // 299 - För lång svarvstångsrotation : aktivera läget Hempositionskommando
    // 300 - Lång svarvstång T1 riktningsindex
    // 301 - Lång svarvstång T3 riktningsindex
    // 302 - 3D koordinatkonvertering ångra med kommando
    // 303 - 3D koordinatkonverteringskommando
    // 304 - Ignorera enskilt block FRÅN
    // 305 - Ignorera enskilt block TILL
    // 306 - Optimal tidskonstant värdefunktion FRÅN
    // 307 - Optimal tidskonstant värdefunktion TILL
    // 310 - Bearbetning med konstant skärhastighet för revolver C
    // 311 - Bearbetning med konstant skärhastighet för revolver D
    param.g[g_use_subsystem_c] = {313};
    param.g[g_use_subsystem_d] = {314};
    // 322 - W-axelkommando för subspindeln på revolver C
    // 323 - W-axelkommando för subspindeln på revolver D
    // 330 - WHILE
    // 331 - DO
    // 332 - END
    // 333 - THEN
    // 444 - Verktygsställningens fluktuationsreduceringsläge FRÅN
    // 445 - Verktygsställningens fluktuationsreduceringsläge TILL
    // 467 - 3-D Koordinat indexkommando
    // 468 - 3-D koordinatkonvertering ångra
    // 469 - 3-D Koordinatkonvertering

    param.g[g_auto_geometry_function] = {std::set<int>{
            178, 179, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191}};

    // Define M-codes
    param.m[m_program_stop] = {0};
    param.m[m_optional_stop] = {1};
    param.m[m_end_of_program] = {2};
    param.m[m_spindle_cw] = {3};
    param.m[m_spindle_ccw] = {4};
    param.m[m_spindle_off] = {5};
    // 6 - Verktygsbyte
    param.m[m_coolant_1_on] = {8};
    param.m[m_coolant_1_off] = {9};
    param.m[m_queueing] = {10}; // Krypkörning av spindel FRÅN?
    // 11 - Krypkörning av spindel TILL
    param.m[m_revolver_stop] = {12};
    // M13?
    param.m[m_revolver_cw] = {13};
    param.m[m_revolver_ccw] = {14};
    param.m[m_c_axis_rotation_right] = {15};
    param.m[m_c_axis_rotation_left] = {16};
    param.m[m_data_transfer_request] = {17};
    // 18 - avbryt spindelorientering
    // 19 - spindelorientering
    param.m[m_double_dock_barrier_off] = {20};
    param.m[m_double_dock_barrier_on] = {21};
    param.m[m_thread_chamfer_off] = {22};
    param.m[m_thread_chamfer_on] = {23};
    param.m[m_chuck_barrier_off] = {24};
    param.m[m_chuck_barrier_on] = {25};
    param.m[m_thread_pitch_axis_z] = {26},
    param.m[m_thread_pitch_axis_x] = {27},
    // 28 - Funktion för kontroll av verktygskollision FRÅN
    // 29 - Funktion för kontroll av verktygskollision TILL
    param.m[m_end_of_data] = {30};
    param.m[m_thread_cut_straight_left] = {32};
    param.m[m_thread_cut_zigzag] = {33};
    param.m[m_thread_cut_straight_right] = {34};
    // 35 - Laddarmanipulator, återdragning av Z-slid
    // 36 - Laddarmanipulator, framkörning av Z-slid
    // 37 - Återdragning av laddararm
    // 38 - Framkörning av laddararm till avlastningsposition
    // 39 - Framkörning av laddararm till chuckposition
    param.m[m_spindle_gear_neutral] = {40};
    param.m[m_spindle_gear_1] = {41};
    param.m[m_spindle_gear_2] = {42};
    param.m[m_spindle_gear_3] = {43};
    param.m[m_spindle_gear_4] = {44};
    // 45 - OSL urladdararm kör gripdon i riktning bakåt
    // 46 - OSL urladdararm kör gripdon i framåtriktning
    param.m[m_bypass_override_off] = {48};
    param.m[m_bypass_override] = {49};
    param.m[m_compressed_air_2_off] = {50};
    param.m[m_compressed_air_2_on] = {51};
    // 54 - Automatisk indexering av indexchuck
    param.m[m_nubdock_retract] = {55};
    param.m[m_nubdock] = {56};
    // 57 - Upphäva M63
    // 58 - Lågt chucktryck
    // 59 - Högt tryck till chuck
    param.m[m_feed_rate_wait] = {60};
    param.m[m_feed_rate_nowait] = {61};
    // 62 - Upphäva M64
    param.m[m_spindle_rotation_nowait] = {63};
    // 64 - Ignorera svar från allmän M-kod
    // 65 - Ignorerar svar från T-kod
    // 66 - Revolverns indexeringsposition fri
    // 67 - Upphäva synkronisering vid kamsvarvningscykel
    // 68 - Synkroniseringsläge A TILL vid kamsvarvningscykel
    // 69 - Synkroniseringsläge B TILL vid kamsvarvningscykel
    // 70 - Manuellt verktygsbyteskommando (typ ATC)
    param.m[m_thread_cycle_schema_1] = {73}; // Need special code
    param.m[m_thread_cycle_schema_2] = {74}; // Need special code
    param.m[m_thread_cycle_schema_3] = {75}; // Need special code
    // 76 - Återdragning av detaljgripare
    // 77 - Framkörning av detaljgripare
    // 78 - Stöddocka 1 frigör
    // 79 - Stöddocka 1 lås
    // 80 - Överskärning fram
    // 81 - Överskärning tillbaka
    // 82 - Spånfriläggning detektering TILL
    param.m[m_clamp_workpiece] = {83};
    param.m[m_unclamp_workpiece] = {84};
    param.m[m_scrub_cycle_noreturn] = {85};
    param.m[m_revolver_cw_locked] = {86};
    param.m[m_revolver_cw_unlocked] = {87};
    param.m[m_compressed_air_1_off] = {88};
    param.m[m_compressed_air_1_on] = {89};
    // 90 - Stänga kåpa
    // 91 - Öppna kåpa
    // 92 - Återdragning av stångmatare
    // 93 - Framkörning av stångmatare
    // 94 - Pålastning av laddare
    // 95 - Urlastning av laddare
    // 96 - Återdragning av detaljgripare för extraspindel
    // 97 - Delfångare för sub-spindel framåt
    // 98 - Dubbdockshastighet låg
    // 99 - Dubbdockshastighet hög
    // 100 - Buffring förbjuden, väntar på synkroniseringskommando
    // 101-108 - Extern M-kod
    param.m[m_spindle_on_axis_c_off] = {109};
    param.m[m_spindle_on_axis_c_on] = {110};
    // 111 - Autoinställning av friläggande av axelns nollpunktsförskjutning
    // 112 - M-verktygsspindel på 3dje revolverstoppet
    // 113 - M-verktygsspindel på 3dje revolvern, framåtrotation
    // 114 - M-verktygsspindel på 3dje revolvern, bakåtrotation
    // 115 - Urladdare grip öppen
    // 116 - Urladdare, grip stäng
    // 117 - Framkörning av sensorhuvud
    // 118 - Återdragning av sensorhuvud
    // 119 - Speciell arbetsstycksräkning
    // 120 - Inget arbetsstycke bekräftat
    // 121 - Öppna/stänga lynett
    // 122 - Dra tillbaka lynett
    // 123 - Stöddocka framåt
    param.m[m_alarm_on_stm_time_passed_on] = {124};
    param.m[m_alarm_on_stm_time_passed_off] = {125};
    param.m[m_compressed_air_4_off] = {126};
    param.m[m_compressed_air_4_on] = {127};
    // 128 - Återdra dubbdockans sväng
    // 129 - Kör fram dubbdockans sväng
    // 130 - Chuckfelsdetektering luft FRÅN
    // 131 - Chuckfelsdetektering luft TILL
    // 132 - Detektering av uppspänningsfel i chuck FRÅN
    // 133 - Detektering av uppspänningsfel i chuck TILL
    // 134 - Z-axel tryckövervakning FRÅN
    // 135 - Z-axel tryckövervakning TILL
    param.m[m_profile_for_fixed_cycle] = {136};
    // 137 - Frigöring av touch setterns förregling TILL
    // 138 - Frigöring av touch setterns förregling FRÅN
    // 139 - CAM svarv inlärningskontroll TILL
    param.m[m_thread_tool_rpm_nowait] = {140};
    param.m[m_c_axis_use_locking] = {141};
    // 142 - Lågt kylmedelstryck
    // 143 - Högt kylmedelstryck
    // 144 - Extra kylmedel 1 FRÅN
    // 145 - Extra kylmedel 1 TILL
    param.m[m_c_axis_unlock] = {146};
    param.m[m_c_axis_lock] = {147};
    // 150 - Synkroniserad rotation FRÅN
    // 151 - Synkroniserad rotation TILL
    param.m[m_revolver_allowed_without_axis_c_off] = {152};
    param.m[m_revolver_allowed_without_axis_c_on] = {153};
    param.m[m_compressed_air_3_off] = {154};
    param.m[m_compressed_air_3_on] = {155};
    param.m[m_nub_workmode_on] = {156};
    param.m[m_nub_workmode_off] = {157};
    // 158 - Cam-svarvning, sykroniserad drift FRÅN
    // 159 - Cam-svarvning, synkroniserad drift TILL
    param.m[m_feed_limit_manual_obey_on] = {160};
    param.m[m_feed_limit_manual_obey_off] = {161};
    param.m[m_rpm_limit_manual_obey_on] = {162};
    param.m[m_rpm_limit_manual_obey_off] = {163};
    param.m[m_single_block_obey_on] = {164};
    param.m[m_single_block_obey_off] = {165};
    param.m[m_nub_allowed_on_spindle_rotation_off] = {166};
    param.m[m_nub_allowed_on_spindle_rotation_on] = {167};
    // 168 - Överföringsläge FRÅN (Dammuppsamlarlucka UPPFÄLLD)
    // 169 - Överföringsläge TILL (Dammuppsamlarlucka NEDFÄLLD)
    // 170 - Friläggning av C-Axelns högtryckbroms TILL
    // 171 - Friläggning av C-Axelns högtryckbroms FRÅN
    // 172 - Frigöra förregling när roboten befinner sig inne i svarven FRÅN
    // 173 - Frigöra förregling när roboten befinner sig inne i svarven TILL
    // 174 - Extra kylmedel 2 FRÅN
    // 175 - Extra kylmedel 2 TILL
    // 176 - Lossa Y-axel
    // 177 - Lås Y-axel
    // 178 - Plugg framåtflyttning
    // 179 - Plugg återdragning
    // 180 - Robot/laddare begäran 1
    // 181 - Robot/laddare begäran 2
    // 182 - Robot/laddare begäran 3
    // 183 - Robot/laddare begäran 4
    param.m[m_chuck_opening_allowed_on_spindle_rotation_off] = {184};
    param.m[m_chuck_opening_allowed_on_spindle_rotation_on] = {185};
    // 186 - Tryckluft F FRÅN
    // 187 - Tryckluft F TILL
    // 188 - Dubbdockskoppling FRÅN
    // 189 - Dubbdockskoppling TILL
    // 190 - Möjligt att bestämma G00 med koppling av dubbdocka
    // 191 - M-verktyg spindelorienteringsriktning angiven MEDURS
    // 192 - M-verktyg spindelorienteringsriktning angiven MOTURS
    // 193 - Upphäva M194
    // 194 - Fasning för gängskärning
    // m_thread_cycle_save_manual_phase_compensation - on - 196
    // m_thread_cycle_save_manual_phase_compensation - off - 195
    // m_thread_cycle_save_manual_phase_compensation - delete - 197
    // 198 - Överskridning av STM cykeltid TILL
    // 199 - Överskridning av STM cykeltid FRÅN
    // 200 - Upphäva synkroniserad matning för Z-axel
    // 201 - Z-axel synkroniserad matning G13
    // 202 - Z-axel synkroniserad matning G14
    // 203 - Frigöring av revolver
    // 204 - ATC-slutare stäng
    // 205 - ATC slutare öppen
    // 206 - A/B-sadel synkroniserad matning FRÅN
    // 207 - A/B-sadel synkroniserad matning TILL
    // 208 - Dörrförregling C,D TILL
    // 209 - Frigör luckförregling C,D FRÅN
    // 210 - Avbryt all fasning för synkroniserad rotation eller plansvarvning
    param.m[m_wedge_track_cycle_single_dimension] = {211};
    param.m[m_wedge_track_cycle_single_zigzag] = {212};
    param.m[m_wedge_track_cycle_specific_cutting_amount] = {213};
    param.m[m_wedge_track_cycle_equal_cutting_amount] = {214};
    // 215 - Ignorera belastningsövervakning G00 FRÅN
    // 216 - Ignorera belastningsövervakning G00 TILL
    // 217 - M-verktygsstopp med orientering
    // 218 - Tryckluft E FRÅN
    // 219 - Tryckluft E TILL
    // 220 - Plansvarvning FRÅN
    // 221 - Plansvarvning FRÅN (1:1)
    // 222 - Plansvarvning FRÅN (1:2)
    // 223 - Plansvarvning FRÅN (1:3)
    // 224 - Plansvarvning FRÅN (1:4)
    // 225 - Plansvarvning FRÅN (1:5)
    // 226 - Plansvarvning FRÅN (1:6)
    // 227 - ATC-drift fullborda, vänt-kommando
    // 228 - ATC återlämning av nästa verktyg
    // 229 - AM-verktyg spindelorientering
    // 230-237 - Extern M-signal
    // 238 - Plansvarvning M-spindel, fasvariation
    // 239 - Plansvarvningsspindel, orientering
    // 240 - Stöddocka 2 stäng slut
    param.m[m_multiop_rpm_min] = {241};
    param.m[m_multiop_rpm_max] = {242};
    param.m[m_chip_transport_off] = {243};
    param.m[m_chip_transport_on] = {244};
    // 245 - X axelns snabbmatning ACC. enhetsändring
    // 246 - Plansvarvningsspindel frigör chuckförregling FRÅN
    // 247 - Plansvarvningsspindel frigör chuckförregling TILL
    // 248 - Plansvarvningsspindel chucklåsning
    // 249 - Plansvarvningsspindel chuckfrigörning
    // 250 - Återdra utstötare för arbetsstycke
    // 251 - Köra fram utstötare för arbetsstycke
    // 252 - Skriva ut data från laser interferometer
    // 253 - Kontrolleraa data från laser interferometer
    // 254 - Arbete ut shooter framåt
    // 255 - Arbete ut shooter retur
    // 256 - Robot/laddare begäran 5
    // 257 - Robot/laddare begäran 6
    // 258 - Robot/laddare begäran 7
    // 259 - Robot/laddare begäran 8
    // 260 - Stegmatningsläge FRÅN
    // 261 - Stegmatningsläge TILL
    // 262 - Extra kylvätska 3 FRÅN/Verktyg V kylvätska FRÅN
    // 263 - Extra kylvätska 3 TILL/Verktyg V kylvätska TILL
    // 264 - Begränsning för överlappningsläge, frigör snabbmatning FRÅN
    // 265 - Begränsning för överlappningsläge, frigör snabbmatning TILL
    // 266 - R spindel spännhylsekontroll stängd
    // 267 - R spindel spännhylsecheck öppen
    // 268 - Dubbstocksframåtkörning i läge omöjligt
    // 269 - Optisk sensor ström FRÅN
    // 270 - Optisk sensor start/stopp, optisk sensor ström på
    // 271 - Spindel extremt lågt rotationsläge TILL
    // 272 - Spindel extremt lågt rotationsläge FRÅN
    // 273 - 2:a programgräns aktiv
    // 274 - 2:a programgräns inaktiv
    // 275 - ATC Särskild sekvens
    // 276 - ATC 1:a slutare stäng
    // 277 - ATC 1:a slutare öppen
    // 278 - Stöddocka 2 frigör
    // 279 - Stöddocka 2 lås
    // 280 - M280 avbryt
    // 281 - lasersensor aktiverad
    // 282 - Skjut tillbaka steg
    // 283 - Automatiskt framåtkörning
    // 284 - Tryckluft i chuck
    // 285 - arbetsräkning
    // 286 - Stift framåt (insatshylsa chuck stäng)
    // 287 - Stift återdragning (insatshylsa chuck öppen)
    // 288 - Sub-spindel tryckluft FRÅN
    // 289 - Sub-spindel tryckluft TILL
    // 290 - Stäng taklucka
    // 291 - Öppna taklucka
    // 292 - Återdragning av urladdare
    // 293 - Framåtkörning av urladdare
    // 294 - Ingen termisk avvikelse gällande dataöverföring
    // 295 - Avbryt termisk avvikelse för dataöverföring
    // 296 - Tid för konstant omkoppling (för mindre skärmärken)
    // 297 - Tid för konstant omkoppling (för effektiva skäroperationer)
    // 298 - Avbrutet av M299
    // 299 - Avbryt repetitionsläge
    // 300 - Manipulator A stäng
    // 301 - Manipulator A öppen
    // 302 - Handindex A-position
    // 303 - Handindex B-position
    // 306 - Arbetsstyckets stötstång A framåtkörning
    // 307 - Arbetsstyckets stötstång A återdragning
    // 308 - Svänghand A
    // 309 - Svänghand B

    // TODO complete list
    param.m[m_spindle_rpm_variable_off] = {694};
    param.m[m_spindle_rpm_variable_on] = {695};

    return param;
}

}

