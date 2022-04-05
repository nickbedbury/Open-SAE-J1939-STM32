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

| File | Description |
| ------------------------------- | -------------- |
| Inc/stm32f7xx_hal_conf.h        | HAL configuration file |
| Inc/stm32f7xx_it.h              | DMA interrupt handlers header file |
| Inc/main.h                      | Header for main.c module |
| Inc/Hardware.h                  | Open-J1939 Hardware.h implementation |
| Inc/Open_SAE_J1939.h            | Open-J1939 main header |
| Inc/Struct.h                    | Definitions of various data types for Open-J1939 |
| Src/stm32f7xx_it.c              | DMA interrupt handlers | 
| Src/main.c                      | Main program | 
| Src/stm32f7xx_hal_msp.c         | HAL MSP file | 
| Src/system_stm32f7xx.c          | STM32F7xx system source file |
| Src/CAN_Transmit_Receive.c      | Open-J1939 CAN function wrappers |
| Src/FLASH_EEPROM_RAM_Memory.c   | Open-J1939 memory management |
| Src/Listen_For_Messages.c       | Open-J1939 listener |
| Src/Save_Load_Struct.c          | Open-J1939 custom settings implementation |
| Src/Startup_ECU.c               | Open-J1939 startup functions |
| ISO_11783                       | Copy of Open-J1939 |
| SAE_J1939                       | Copy of Open-J1939 |
  
# Hardware and Software environment

 - This example runs on STM32F767xx/STM32F769xx/STM32F777xx/STM32F779xx devices,
   essentially those with a built-in CAN peripheral.
 - The example may require an external CAN transceiver if the eval board does 
   not have one.  This is an overview of how to construct the necessary circuit:
   https://www.instructables.com/STM32-CAN-Interface/
 - This example has NOT been tested with STM32F769I-EVAL board, but should be
   easily tailored to any supported device and development board.

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
