#include <gtest/gtest.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <pcap/pcap.h>

#include "../src/utils/log/Log.h"

using namespace faker_tsn;

/* callable function */
static void my_callback(u_char* useless, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    static int count = 1;
    printf("Packet %d:\n", count);
    printf("    useless: %x\n", useless);                                     /* 句柄 */
    printf("    pkthdr: %x\n", pkthdr);                                       /* 收到数据包的 pcap_pkthdr 类型的指针 */
    printf("    pkthdr->ts: %d.%d\n", pkthdr->ts.tv_sec, pkthdr->ts.tv_usec); /* 时间戳 */
    printf("    pkthdr->caplen: %d\n", pkthdr->caplen);                       /* 抓到的数据长度 */
    printf("    pkthdr->len: %d\n", pkthdr->len);                             /* 数据包的实际长度 */
    printf("    packet: %x\n", packet);                                       /* 收到的数据包数据 */

    if (count >= 3) {
        pcap_breakloop((pcap_t*)useless);
    }
    count += 1;
}

static void TestPCAPLookUpDev() {
    char *dev, errbuf[PCAP_ERRBUF_SIZE];
    /* find default interface */
    dev = pcap_lookupdev(errbuf);
    if (dev == NULL) {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
        exit(EXIT_FAILURE);
    }
    INFO("Device: " + std::string(dev));
}

void helper(sockaddr* _addr, const char* info) {
    if (_addr) {
        fprintf(stdout, "%s->sa_family: %d\n", info, _addr->sa_family);
        if (_addr->sa_family == PF_PACKET) {  // link layer packet
            sockaddr_ll* addr = (sockaddr_ll*)_addr;
            fprintf(stdout, "%s->sll_addr:", info);
            for (int i = 0; i < 8; i++) {
                fprintf(stdout, "%.2X ", (unsigned char)addr->sll_addr[i]);
            }
            puts("");
        } else if (_addr->sa_family == PF_INET) {  // ipv4 packet
            sockaddr_in* addr = (sockaddr_in*)_addr;
            char* ip;
            ip = inet_ntoa(addr->sin_addr);  // return the IP
            fprintf(stdout, "%s->sin_addr.s_addr: %s", info, ip);
            puts("");
        }
    }
}

static void TestPCAPFindAllDevs() {
    pcap_if_t* devs;
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_findalldevs(&devs, errbuf) < 0) {
        fprintf(stderr, "Couldn't open devices: %s\n", errbuf);
    }
    // devices
    for (pcap_if_t* dev = devs; dev != NULL; dev = dev->next) {
        fprintf(stdout, "name: %s\n", dev->name);
        fprintf(stdout, "flags: %d\n", dev->flags);
        puts("");
        // sa_family: 17 是链路层包，2 是 ipv4 包，10 是 ipv6 包
        for (pcap_addr_t* addr = dev->addresses; addr != NULL; addr = addr->next) {
            /* print addr、netmask、broadaddr、dstaddr */
            fprintf(stdout, "addr->addr: 0x%x %p\n", addr->addr, addr->addr);
            helper(addr->addr, "addr->addr");
            fprintf(stdout, "addr->netmask: 0x%x %p\n", addr->netmask, addr->netmask);
            helper(addr->netmask, "addr->netmask");
            fprintf(stdout, "addr->broadaddr: 0x%x %p\n", addr->broadaddr, addr->broadaddr);
            helper(addr->broadaddr, "addr->broadaddr");
            fprintf(stdout, "addr->dstaddr: 0x%x %p\n", addr->dstaddr, addr->dstaddr);
            helper(addr->dstaddr, "addr->dstaddr");
            puts("");
        }
        fprintf(stdout, "..........................................................\n");
    }

    pcap_freealldevs(devs);
}

static void TestPCAPLoop() {
    /* open the session on non-promiscouos mode */
    char* dev = "ens33";
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle;
    handle = pcap_open_live(dev, BUFSIZ, 0, -1, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        exit(EXIT_FAILURE);
    }
    printf("Handle: %x\n", (u_char*)handle);

    /* find the properties for the device */
    bpf_u_int32 mask; /* Our netmask */
    bpf_u_int32 net;  /* Our IP */
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
        net = 0;
        mask = 0;
    }

    /* compile and apply the filter */
    struct bpf_program fp;                  /* The compiled filter */
    char filter_exp[] = "portrange 0-1023"; /* The filter expression */
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        exit(EXIT_FAILURE);
    }

    /* loop */
    pcap_loop(handle, 5, my_callback, (u_char*)handle);  

    fprintf(stdout, "\nDone processing packets... wheew!\n");

    pcap_close(handle);
}

TEST(TEST_LIBPCAP, TEST_PCAP_LOOK_UP_DEV) {
    TestPCAPLookUpDev();
}

TEST(TEST_LIBPCAP, TEST_PCAP_FIND_ALL_DEVS) {
    TestPCAPFindAllDevs();
}

TEST(TEST_LIBPCAP, TEST_PCAP_LOOP) {
    TestPCAPLoop();
}