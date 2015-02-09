/*
 * Copyright (C) 2015 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @{
 *
 * @file
 * @brief       microcoap example server
 *
 * @author      Lotte Steenbrink <lotte.steenbrink@haw-hamburg.de>
 *
 * @}
 */

#include <stdio.h>

#include "udp.h"
#include "net_help.h"
#include "net_if.h"

#include <coap.h>

#define ENABLE_DEBUG    (0)
#include "debug.h"

#define PORT 5683
#define BUFSZ 128

#define RCV_MSG_Q_SIZE      (64)

msg_t msg_q[RCV_MSG_Q_SIZE];
static ipv6_addr_t prefix;

int sock_rcv, if_id;
sockaddr6_t sa_rcv;
uint8_t buf[BUFSZ];
uint8_t scratch_raw[BUFSZ];
coap_rw_buffer_t scratch_buf = {scratch_raw, sizeof(scratch_raw)};

static void _init_tlayer(void);
static void _init_socket(void);
static uint16_t get_hw_addr(void);

int main(void)
{

    DEBUG("Starting example microcoap server...\n");

    _init_tlayer();
    _init_socket();

    while(1)
    {
        int n, rc;
        socklen_t len = sizeof(sa_rcv);
        coap_packet_t pkt;

        n = socket_base_recvfrom(sock_rcv, buf, sizeof(buf), 0, &sa_rcv, &len);
        printf("Received packet: ");
        coap_dump(buf, n, true);
        printf("\n");

        if (0 != (rc = coap_parse(&pkt, buf, n)))
            printf("Bad packet rc=%d\n", rc);
        else
        {
            size_t rsplen = sizeof(buf);
            coap_packet_t rsppkt;
            printf("content:\n");
            coap_dumpPacket(&pkt);
            coap_handle_req(&scratch_buf, &pkt, &rsppkt);

            if (0 != (rc = coap_build(buf, &rsplen, &rsppkt)))
                printf("coap_build failed rc=%d\n", rc);
            else
            {
                printf("Sending packet: ");
                coap_dump(buf, rsplen, true);
                printf("\n");
                printf("content:\n");
                coap_dumpPacket(&rsppkt);
                socket_base_sendto(sock_rcv, buf, rsplen, 0, &sa_rcv, sizeof(sa_rcv));
            }
        }
    }
    return 0;
}

static uint16_t get_hw_addr(void)
{
    return sysconfig.id;
}

/* init transport layer & routing stuff*/
static void _init_tlayer()
{
    msg_init_queue(msg_q, RCV_MSG_Q_SIZE);

    net_if_set_hardware_address(0, get_hw_addr());

    printf("initializing 6LoWPAN...\n");

    ipv6_addr_init(&prefix, 0xABCD, 0xEF12, 0, 0, 0, 0, 0, 0);
    if_id = 0; // >1 interface isn't supported anyway, so there

    sixlowpan_lowpan_init_interface(if_id);
}

static void _init_socket() 
{
    sa_rcv = (sockaddr6_t) { .sin6_family = AF_INET6,
               .sin6_port = HTONS(PORT) };

    int sock_rcv = socket_base_socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    
    if (-1 == socket_base_bind(sock_rcv, &sa_rcv, sizeof(sa_rcv))) {
        printf("Error: bind to receive socket failed!\n");
        socket_base_close(sock_rcv);
    }
}
