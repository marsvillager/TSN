#ifndef _R_TAG_H
#define _R_TAG_H

#include <linux/types.h>
#include <netinet/if_ether.h>

#define VLAN_HLEN 4      /* The additional bytes required by VLAN \
                          * (in addition to the Ethernet header)  \
                          */
#define VLAN_ETH_HLEN 18 /* Total octets in header.       */
#define VLAN_ETH_ZLEN 64 /* Min. octets in frame sans FCS */

/*
 * According to 802.3ac, the packet can be 4 bytes longer. --Klika Jan
 */
#define VLAN_ETH_DATA_LEN 1500  /* Max. octets in payload        */
#define VLAN_ETH_FRAME_LEN 1518 /* Max. octets in frame sans FCS */

#define RTAG_VLAN_HLEN 6      /* The additional bytes required by RTAG  \
                               * (in addition to the IEEE802.1Q header) \
                               */
#define RTAG_VLAN_ETH_HLEN 24 /* Total octets in header.       */
#define RTAG_VLAN_ETH_ZLEN 64 /* Min. octets in frame sans FCS */

#define RTAG_VLAN_ETH_DATA_LEN 1500  /* Max. octets in payload        */
#define RTAG_VLAN_ETH_FRAME_LEN 1518 /* Max. octets in frame sans FCS */

/*
 *  struct vlan_hdr - vlan header
 *  @h_vlan_TCI: priority and VLAN ID
 *  @h_vlan_encapsulated_proto: packet type ID or len
 */
struct vlan_hdr {
    __be16 h_vlan_TCI;
    __be16 h_vlan_encapsulated_proto;
} __attribute__((packed, aligned(1)));

/*
 *  struct rtag_hdr - rtag header
 *  @h_rtag_proto: ethernet protocol
 *  @h_rtag_rsvd: reserved field
 *  @h_rtag_seq_num: sequence number
 *  @h_rtag_encapsulated_proto: packet type ID or len
 */
struct rtag_hdr {
    __be16 h_rtag_rsved;
    __be16 h_rtag_seq_num;
    __be16 h_rtag_encapsulated_proto;
} __attribute__((packed, aligned(1)));

/**
 *  struct vlan_ethhdr - vlan ethernet header (ethhdr + vlan_hdr)
 *  @h_dest: destination ethernet address
 *  @h_source: source ethernet address
 *  @h_vlan_proto: ethernet protocol
 *  @h_vlan_TCI: priority and VLAN ID
 *  @h_vlan_encapsulated_proto: packet type ID or len
 */
struct vlan_ethhdr {
    unsigned char h_dest[ETH_ALEN];
    unsigned char h_source[ETH_ALEN];
    __be16 h_vlan_proto;
    __be16 h_vlan_TCI;
    __be16 h_vlan_encapsulated_proto;
} __attribute__((packed, aligned(1)));

/**
 *  struct rtag_vlan_ethhdr - rtag vlan ethernet header (ethhdr + vlan_hdr + rtag_hdr)
 *  @h_dest: destination ethernet address
 *  @h_source: source ethernet address
 *  @h_vlan_proto: ethernet protocol
 *  @h_vlan_TCI: priority and VLAN ID
 *  @h_rtag_proto: ethernet protocol
 *  @h_rtag_rsvd: reserved field
 *  @h_rtag_seq_num: sequence number
 *  @h_rtag_encapsulated_proto: packet type ID or len
 */
struct rtag_vlan_ethhdr {
    unsigned char h_dest[ETH_ALEN];
    unsigned char h_source[ETH_ALEN];
    __be16 h_vlan_proto;
    __be16 h_vlan_TCI;
    __be16 h_rtag_proto;
    __be16 h_rtag_rsvd;
    __be16 h_rtag_seq_num;
    __be16 h_rtag_encapsulated_proto;
} __attribute__((packed, aligned(1)));

/**
 * struct vlan_tci - TCI (tag contorl information) in vlan tag
 * @pcp: priority code point
 * @dei: Drop eligible indicator
 * @vid: VLAN identifier
 */
struct vlan_tci {
    __be16 pcp : 3;
    __be16 dei : 1;
    __be16 vid : 12;
} __attribute__((packed, aligned(1)));

struct tsn_hdr {
    struct ethhdr eth_hdr;
    struct vlan_hdr vlan_tag;
    struct rtag_hdr r_tag;
} __attribute__((packed, aligned(1)));

union tsn_frame {
    struct {
        struct tsn_hdr header;
        unsigned char data[ETH_DATA_LEN];
    } __attribute__((packed, aligned(1))) filed;
    unsigned char buffer[ETH_FRAME_LEN];
} __attribute__((packed));

#endif