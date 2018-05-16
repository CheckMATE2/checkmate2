#include "Units.h"

namespace units {
    double strToUnit(std::string unit) {
        if (unit == "KB") return KB;
        else if (unit == "B") return B;
        else if (unit == "MB") return MB;
        else if (unit == "MUB") return MUB;
        else if (unit == "NB") return NB;
        else if (unit == "PB") return PB;
        else if (unit == "FB") return FB;
        else if (unit == "AB") return AB;
        else if (unit == "ZB") return ZB;
        else if (unit == "INVKB") return INVKB;
        else if (unit == "INVB") return INVB;
        else if (unit == "INVMB") return INVMB;
        else if (unit == "INVMUB") return INVMUB;
        else if (unit == "INVNB") return INVNB;
        else if (unit == "INVPB") return INVPB;
        else if (unit == "INVFB") return INVFB;
        else if (unit == "INVAB") return INVAB;
        else if (unit == "INVZB") return INVZB;
        else {std::cerr << "Unit unknown!" << std::endl; exit(1);}
    }
}