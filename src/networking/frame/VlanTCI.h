#ifndef _VLAN_TCI_H
#define _VLAN_TCI_H

#include <memory.h>
#include <stdint.h>
#include <sstream>
#include <string>

#include "../../utils/ConvertUtils.h"
#include "if_ether.h"

namespace faker_tsn {

enum {
    PCP_0 = 0x0000,  // 0    0000 0000 0000 0000
    PCP_1 = 0x0020,  // 1    0010 0000 0000 0000
    PCP_2 = 0x0040,  // 2    0100 0000 0000 0000
    PCP_3 = 0x0060,  // 3    0110 0000 0000 0000
    PCP_4 = 0x0080,  // 4    1000 0000 0000 0000
    PCP_5 = 0x00A0,  // 5    1010 0000 0000 0000
    PCP_6 = 0x00C0,  // 6    1100 0000 0000 0000
    PCP_7 = 0x00E0,  // 7    1110 0000 0000 0000
};

enum {
    DEI_0 = 0x0000,  // 0    0000 0000 0000 0000
    DEI_1 = 0x0010,  // 1    0000 0000 0000 0001
};

struct VlanTCI {
    // N    0x2001 : 0010 0000 , 0000 0001
    // H    0x0120 : 0000 0001 , 0010 0000
    uint8_t pcp;  /* pritority code point */
    uint8_t dei;  /* drop dligible indicator */
    uint16_t vid; /* vlan id */

    std::string toString() {
        std::stringstream ss;
        ss << "pcp = " << this->pcp << " ";
        ss << "del = " << this->dei << " ";
        ss << "vid = " << this->vid;
        return ss.str();
    }

    /* encode VlanTCI to raw TCI (network byte order) */
    static __be16 encode(struct VlanTCI vlan_tci) {
        __be16 tci = 0x0000;

        if (vlan_tci.pcp == 1) {
            tci |= PCP_1;
        } else if (vlan_tci.pcp == 2) {
            tci |= PCP_2;
        } else if (vlan_tci.pcp == 3) {
            tci |= PCP_3;
        } else if (vlan_tci.pcp == 4) {
            tci |= PCP_4;
        } else if (vlan_tci.pcp == 5) {
            tci |= PCP_5;
        } else if (vlan_tci.pcp == 6) {
            tci |= PCP_6;
        } else if (vlan_tci.pcp == 7) {
            tci |= PCP_7;
        } else {
            tci |= PCP_0;
        }

        if (vlan_tci.dei == 1) {
            tci |= DEI_1;
        }

        // TODO check the range of vlan id

        __be16 vid = htons(vlan_tci.vid);
        tci |= vid;

        return tci;
    }

    /* decode raw TCI (network byte order) to VlanTCI */
    static VlanTCI
    parse(uint16_t tci) {
        VlanTCI vlan_tci;

        uint8_t _pcp = tci & 0x00E0;  // 1110 0000 0000 0000
        if (_pcp == PCP_0)
            vlan_tci.pcp = 0;
        else if (_pcp == PCP_0)
            vlan_tci.pcp = 0;
        else if (_pcp == PCP_1)
            vlan_tci.pcp = 1;
        else if (_pcp == PCP_2)
            vlan_tci.pcp = 2;
        else if (_pcp == PCP_3)
            vlan_tci.pcp = 3;
        else if (_pcp == PCP_4)
            vlan_tci.pcp = 4;
        else if (_pcp == PCP_5)
            vlan_tci.pcp = 5;
        else if (_pcp == PCP_6)
            vlan_tci.pcp = 6;
        else if (_pcp == PCP_7)
            vlan_tci.pcp = 7;

        uint8_t _dei = tci & 0x0010;  // 0001 0000 0000 0000
        if (_dei == DEI_0)
            vlan_tci.dei = 0;
        else
            vlan_tci.dei = 1;

        uint16_t _vid = tci & 0xFF0F;  // 0000 1111 1111 1111
        _vid = ntohs(_vid);
        vlan_tci.vid = (_vid);

        return vlan_tci;
    }
};

}  // namespace faker_tsn

#endif  // _VLAN_TCI_H