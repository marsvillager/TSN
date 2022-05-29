#ifndef _RELAY_ENTITY_H
#define _RELAY_ENTITY_H

namespace faker_tsn {

enum RELAY_ENTITY {
    PACKET_RAW,              // raw packet
    IEEE_802_3_FRAME,        // normal ethner frame
    IEEE_802_1Q_FRAME,       // ether + vlan-tag frame
    IEEE_802_1Q_TSN_FRAME,   // ether + vlan-tag + rtag frame
    IEEE_802_1Q_TSN_FRAME_E  // enhancement tsn frame
};

}  // namespace faker_tsn

#endif  // _RELAY_ENTITY_H