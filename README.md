# FreeRTOS + TLSF memory management example.

This example aims to replace FreeRTOS heap4 memory management sheme with TLSF.

It introduce a new malloc and free wrapper called heap6.c + heap6.h.

# Enviroment

This examples runs on [Firmware v3](https://github.com/epernia/firmware_v3).

# Usage

# Tweaks

* The macro configTOTAL_HEAP_SIZE found at freertosconfig.h still provides the heap size configuration for TLSF
*

