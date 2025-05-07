#ifndef BRANDS_H
#define BRANDS_H

namespace NCParser {

enum Controllers {
    Fanuc,
    Haas,
    Heidenhain,
    Hurco,
    Mitsubishi,
    Siemens,
    Yasnac
};

enum Manufacturers {
    Traub,
    Index,
    Citizen,
    Star,
    Okuma,
};

struct MachineParameters {
    struct comments {
        bool use_parenthesis = true;
        bool use_semicolon = false;
    } comments;
    struct L {
        bool is_variable = false;
    } L;
    struct O {
        bool is_prg_name = false;
    } O;
    struct T {
        bool offset_as_d = false;
    };
    struct P {
        bool is_variable = true;
    };
    struct Q {
        bool is_variable = true;
    };
    struct R {
        bool is_variable = true;
    };
};

};

#endif // BRANDS_H
