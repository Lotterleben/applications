# name of your application
APPLICATION = microcoap-example

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# if you try to compile this for anything but the boards specified, it will break.
# This application has not been verified to work with any other boards-- proceed with caution.
BOARD_WHITELIST := native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../RIOT

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DRIOT -DMICROCOAP_DEBUG

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# pull in the microcoap package
USEPKG=microcoap

# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netif_default
USEMODULE += auto_init_gnrc_netif
# Specify the mandatory networking modules for socket communication via UDP
USEMODULE += gnrc_ipv6_default
USEMODULE += gnrc_udp
USEMODULE += gnrc_conn_udp
USEMODULE += posix_sockets


#USEMODULE += config
#USEMODULE += uart0

#USEMODULE += nativenet TODO: what happened to nativenet?

include $(RIOTBASE)/Makefile.include
