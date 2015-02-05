# name of your application
APPLICATION = microcoap-example

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../RIOT

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP -DDEBUG

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

USEPKG=microcoap

USEMODULE += config

ifeq ($(strip $(BOARD)),native)
	USEMODULE += nativenet
else
	USEMODULE += cc110x
endif

USEMODULE += sixlowpan
USEMODULE += udp

include $(RIOTBASE)/Makefile.include
