/*
 * Copyright (C) 2015 HAW Hamburg
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

/*
#include "udp.h"
#include "net_help.h"
#include "net_if.h"
#include "periph/cpuid.h"
#include "board_uart0.h"
#include "thread.h"
#include "posix_io.h"
#include <coap.h>
#include "hashes.h"
*/

#include "thread.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <coap.h>

#define ENABLE_DEBUG    (1)
#include "debug.h"

#define PORT 5683
#define BUFSZ 128

#define RCV_MSG_Q_SIZE      (64)

static void *_microcoap_server_thread(void *arg);

msg_t msg_q[RCV_MSG_Q_SIZE];
char _rcv_stack_buf[THREAD_STACKSIZE_MAIN];

//static ipv6_addr_t prefix;
int sock_rcv, if_id;
struct sockaddr_in6 sa_rcv;
uint8_t buf[BUFSZ];
uint8_t scratch_raw[BUFSZ];
coap_rw_buffer_t scratch_buf = {scratch_raw, sizeof(scratch_raw)};

static void _init_tlayer(void);

int main(void)
{

    DEBUG("Starting example microcoap server...\n");

    _init_tlayer();
    thread_create(_rcv_stack_buf, sizeof(_rcv_stack_buf), THREAD_PRIORITY_MAIN,
                  THREAD_CREATE_STACKTEST, _microcoap_server_thread, NULL,
                  "_microcoap_server_thread");

    DEBUG("Ready to receive requests.\n");

    return 0;
}

/*
static uint16_t get_hw_addr(void)
{
    return sysconfig.id;
}
*/

/* init transport layer & routing stuff*/
// TODO: do I still need this?
static void _init_tlayer(void)
{
    msg_init_queue(msg_q, RCV_MSG_Q_SIZE);

    /*
    net_if_set_hardware_address(0, get_hw_addr());
    DEBUG("set hawddr to: %d\n", get_hw_addr());

    printf("initializing 6LoWPAN...\n");

    ipv6_addr_init(&prefix, 0xABCD, 0xEF12, 0, 0, 0, 0, 0, 0);
    if_id = 0; *//* having more than one interface isn't supported anyway */

    //sixlowpan_lowpan_init_interface(if_id);

}

static void *_microcoap_server_thread(void *arg)
{
    (void)arg; /* silence warnings about unused variables */


    printf("initializing receive socket...\n");

    sa_rcv = (struct sockaddr_in6) { .sin6_family = AF_INET6,
                                     .sin6_port = HTONS(PORT) };

    sock_rcv = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

    if (-1 == bind(sock_rcv, (struct sockaddr *) &sa_rcv, sizeof(sa_rcv))) {
        printf("Error: bind to receive socket failed!\n");
        close(sock_rcv);
    }

    printf("Ready to receive requests.\n");

    while(1)
    {
        int n, rc;
        socklen_t len = sizeof(sa_rcv);
        coap_packet_t pkt;

        n = recvfrom(sock_rcv, buf, sizeof(buf), 0, (struct sockaddr*) &sa_rcv, &len);
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
                sendto(sock_rcv, buf, rsplen, 0, (struct sockaddr*) &sa_rcv, sizeof(sa_rcv));
            }
        }
    }

    return NULL;
}
