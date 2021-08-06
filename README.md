# FreeRTOS + TLSF memory management example.

This example aims to replace FreeRTOS heap4 memory management scheme with TLSF.

It introduces a new malloc and free wrapper called heap6.c + heap6.h.

It uses a modified version of [TLSF](https://github.com/mattconte/tlsf) where I move all configurations to tlsf_config.h and did some private definitions, public.

# Enviroment

This examples runs on [Firmware v3](https://github.com/epernia/firmware_v3) on EDUCIAA.

# Tweaks

* The macro configTOTAL_HEAP_SIZE found at freertosconfig.h still provides the heap size configuration for TLSF
* All the configurations for TLSF are in tlsf_config.h

