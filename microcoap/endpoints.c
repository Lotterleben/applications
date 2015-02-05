#include <stdbool.h>
#include <stdbool.h>
#include <string.h>
#include "coap.h"

#define ENABLE_DEBUG    (0)
#include "debug.h"

const uint16_t response_len = 1500;
static char response[response_len] = "";

static const coap_endpoint_path_t path = {2, {"foo", "bar"}};

void create_response(void)
{
    strncat(response, "1337", response_len-5);
}

/* The handler which handles the path /foo/bar */
static int handle_get_response(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt, uint8_t id_hi, uint8_t id_lo)
{
    DEBUG("[endpoints]  %s()\n",  __func__);
    create_response();
    /* NOTE: COAP_RSPCODE_CONTENT only works in a packet answering a GET. */
    return coap_make_response(scratch, outpkt, (const uint8_t *)response, strlen(response),
                              id_hi, id_lo, &inpkt->tok, COAP_RSPCODE_CONTENT, COAP_CONTENTTYPE_TEXT_PLAIN);
}

const coap_endpoint_t endpoints[] =
{
    {COAP_METHOD_GET, handle_get_response, &path, "ct=0"},
    {(coap_method_t)0, NULL, NULL, NULL} /* marks the end of the endpoints array */
};
