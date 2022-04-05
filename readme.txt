# Summary

This project is based on STM32Cube's "CAN_Networking" example
  
This example shows how to configure the CAN peripheral to send and receive 
J1939 frames.  By default, the example only sends messages 
if prompted by activity on the bus (i.e. it is more of a listener).

The CAN is configured as follows:
 - Bit Rate   = 1 Mbit/s  
 - CAN Clock  = external clock (HSE)
 - ID Filter  = All identifiers are allowed
 - RTR = Data
 - DLC = 1 byte

# Notes 

If the user code size exceeds the DTCM-RAM size or starts from internal cacheable memories (SRAM1 and SRAM2),that is shared between several processors,
then it is highly recommended to enable the CPU cache and maintain its coherence at application level.
The address and the size of cacheable buffers (shared between CPU and other masters)  must be properly updated to be aligned to cache line size (32 bytes).

It is recommended to enable the cache and maintain its coherence, but depending on the use case
It is also possible to configure the MPU as "Write through", to guarantee the write access coherence.
In that case, the MPU must be configured as Cacheable/Bufferable/Not Shareable.
Even though the user must manage the cache coherence for read accesses.
Please refer to the AN4838 “Managing memory protection unit (MPU) in STM32 MCUs”
Please refer to the AN4839 “Level 1 cache on STM32F7 Series”

# Directory contents 

 - CAN/CAN_Networking/Inc/stm32f7xx_hal_conf.h    HAL configuration file
 - CAN/CAN_Networking/Inc/stm32f7xx_it.h          DMA interrupt handlers header file
 - CAN/CAN_Networking/Inc/main.h                  Header for main.c module  
 - CAN/CAN_Networking/Src/stm32f7xx_it.c          DMA interrupt handlers
 - CAN/CAN_Networking/Src/main.c                  Main program
 - CAN/CAN_Networking/Src/stm32f7xx_hal_msp.c     HAL MSP file
 - CAN/CAN_Networking/Src/system_stm32f7xx.c      STM32F7xx system source file
  
# Hardware and Software environment

 - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices.
 - This example has NOT been tested with STM32F769I-EVAL board, but should be
   easily tailored to any other supported device and development board.

## STM32F769I-EVAL Set-up 
 - Use LED1, LED2, LED3 and LED4 connected respectively to PI.15, PJ.00, PJ.01
   and PJ.03 pins
 - Use Tamper push-button connected to PC.13
 - Connect a male/male CAN cable between at least 2 EVAL CAN connectors (CN26)
 - You have to configure the jumpers as follows:

```
       +-------------+-----------------+
       |   Jumper    |       CAN1      |
       +-------------+-----------------+
       |   JP14      |      fitted     |
       |   JP19      |      1-2        |
       |   JP18      |      fitted     |
       +-------------+-----------------+
```
