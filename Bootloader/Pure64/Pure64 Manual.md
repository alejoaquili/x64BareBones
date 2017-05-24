# Pure64 - v0.6.1 Manual

Pure64 must be loaded to the 16-bit memory address `0x0000:0x8000`

Pure64 expects that the up to 26KiB of data after it is the software that will be loaded to address `0x0000000000100000`.


## System Requirements

A computer with at least one 64-bit Intel or AMD CPU (or anything else that uses the x86-64 architecture)

At least 2 MiB of RAM

The ability to boot via a hard drive, USB stick, or the network


## Hard disk / USB boot

`bmfs_mbr.asm` in the bootsectors folder shows how to boot via a BMFS formatted drive.

*Note*: Once Pure64 has executed you will lose access the the USB drive unless you have written a driver for it. The BIOS was used to load from it and you can't use the BIOS in 64-bit mode.


## Network boot

`pxestart.asm` in the bootsectors folder shows how to build a PXE image.


## Memory Map

This memory map shows how physical memory looks after Pure64 is finished.

| Start Address       | End Address | Size | Description |
|--------------------| -------------------|------------|-------------|
| 0x0000000000000000 | 0x0000000000000FFF | 4 KiB | IDT - 256 descriptors (each descriptor is 16 bytes)| 
| 0x0000000000001000 | 0x0000000000001FFF| 4 KiB | GDT - 256 descriptors (each descriptor is 16 bytes)|
| 0x0000000000002000 | 0x0000000000002FFF | 4 KiB | PML4 - 512 entries, first entry points to PDP at 0x3000|
| 0x0000000000003000 | 0x0000000000003FFF | 4 KiB |PDP - 512 enties|
| 0x0000000000004000 | 0x0000000000007FFF | 16 KiB |  Pure64 Data |
| 0x0000000000008000 | 0x000000000000FFFF |32 KiB | Pure64 - After the OS is loaded and running this memory is free again|
| 0x0000000000010000 |0x000000000004FFFF| 256 KiB | PD - Room to map 64 GiB|
| 0x0000000000050000 |0x000000000009FFFF|320 KiB| Free |
| 0x00000000000A0000|0x00000000000FFFFF|384 KiB| ROM Area| 
| | | | VGA mem at 0xA0000 (128 KiB) Color text starts at 0xB8000|
| | | | Video BIOS at 0xC0000 (64 KiB)|
| | | | Motherboard BIOS at F0000 (64 KiB)|
|0x0000000000100000|0xFFFFFFFFFFFFFFFF| 1+ MiB | Your software is loaded here|

When creating your Operating System or Demo you can use the sections marked free, however it is the safest to use memory above 1 MiB.


## Information Table

Pure64 stores an information table in memory that contains various pieces of data about the computer before it passes control over to the software you want it to load.

The Pure64 information table is located at `0x0000000000005000` and ends at `0x00000000000057FF` (2048 bytes).


|Memory Address|Variable Size | Name | Description |
|--------------|--------------|-------|-------------|
|0x5000        | 64-bits      | ACPI  | Address of the ACPI tables  |
|0x5008        |32-bit        |BSP_ID | APIC ID of the BSP|
|0x5010        |16-bit        |CPUSPEED| Speed of the CPUs in MegaHertz [MHz](http://en.wikipedia.org/wiki/Mhz#Computing) |
|0x5012        |16-bits       |CORES_ACTIVE |The number of CPU cores that were activated in the system|
|0x5014|16-bit|CORES_DETECT|The number of CPU cores that were detected in the system|
|0x5016 - 0x501F| | | For future use|
|0x5020|32-bit|RAMAMOUNT|Amount of system RAM in Mebibytes [MiB](http://en.wikipedia.org/wiki/Mebibyte)|
|0x5022 - 0x502F| | | For future use |
|0x5030|8-bit|IOAPIC_COUNT|Number of IO-APICs in the system|
|0x5031 - 0x503F| | |For future use |
|0x5040|64-bit|HPET|Base memory address for the High Precision Event Timer|
|0x5048 - 0x505F| || For future use|
|0x5060|64-bit|LAPIC|Local APIC address|
|0x5068 - 0x507F|64-bit|IOAPIC|IO-APIC addresses (based on IOAPIC_COUNT)|
|0x5080|32-bit|VIDEO_BASE|Base memory for video (if graphics mode set)|
|0x5084|16-bit|VIDEO_X|X resolution|
|0x5086|16-bit|VIDEO_Y|Y resolution|
|0x5088|8-bit|VIDEO_DEPTH|Color depth|
|0x5100-|8-bit|APIC_ID |APIC ID's for valid CPU cores (based on CORES_ACTIVE)|


A copy of the E820 System Memory Map is stored at memory address `0x0000000000004000`. Each E820 record is 24 bytes in length and the memory map is terminated by a blank record.

|Variable|Variable Size|Description|
|----|----|----|
|Starting Address|64-bit|The starting address for this record|
|Length|64-bit|The length of memory for this record|
|Memory Type|32-bit|Type 1 is usable memory, Type 2 is not usable|


For more information on the E820 Memory Map: [OSDev wiki on E820](http://wiki.osdev.org/Detecting_Memory_%28x86%29#BIOS_Function:_INT_0x15.2C_EAX_.3D_0xE820)
