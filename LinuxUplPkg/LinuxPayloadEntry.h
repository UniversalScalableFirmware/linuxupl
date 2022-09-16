/** @file

  Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __LINUX_PAYLOAD_ENTRY_H__
#define __LINUX_PAYLOAD_ENTRY_H__

#include "linux.h"
#include "Base.h"

//
// Definition of memory status.
//
#define EFI_MEMORY_PRESENT      0x0100000000000000ULL
#define EFI_MEMORY_INITIALIZED  0x0200000000000000ULL
#define EFI_MEMORY_TESTED       0x0400000000000000ULL

#define EFI_MEMORY_UC   0x0000000000000001ULL
#define EFI_MEMORY_WC   0x0000000000000002ULL
#define EFI_MEMORY_WT   0x0000000000000004ULL
#define EFI_MEMORY_WB   0x0000000000000008ULL
#define EFI_MEMORY_UCE  0x0000000000000010ULL

#define EFI_MEMORY_WP  0x0000000000001000ULL
#define EFI_MEMORY_RP  0x0000000000002000ULL
#define EFI_MEMORY_XP  0x0000000000004000ULL
#define EFI_MEMORY_RO  0x0000000000020000ULL
#define EFI_MEMORY_NV  0x0000000000008000ULL
//
// The memory region provides higher reliability relative to other memory in the system.
// If all memory has the same reliability, then this bit is not used.
//
#define EFI_MEMORY_MORE_RELIABLE  0x0000000000010000ULL

#define LEGACY_8259_MASK_REGISTER_MASTER  0x21
#define LEGACY_8259_MASK_REGISTER_SLAVE   0xA1
#define GET_OCCUPIED_SIZE(ActualSize, Alignment) \
  ((ActualSize) + (((Alignment) - ((ActualSize) & ((Alignment) - 1))) & ((Alignment) - 1)))

#define E820_RAM        1
#define E820_RESERVED   2
#define E820_ACPI       3
#define E820_NVS        4
#define E820_UNUSABLE   5
#define E820_DISABLED   6
#define E820_PMEM       7
#define E820_UNDEFINED  8

typedef
void
(*SWITCH_STACK_ENTRY_POINT)(
     void                      *Context1   ,
     void                      *Context2   
  );

typedef struct {
  UINT8     Revision;
  UINT8     Reserved;
  UINT16    Length;
} UNIVERSAL_PAYLOAD_GENERIC_HEADER;

typedef struct {
  UNIVERSAL_PAYLOAD_GENERIC_HEADER    Header;
  UINT32                              Count;
  CHAR8                               CommandLine[0];
} UNIVERSAL_PAYLOAD_LINUX_COMMAND_LINE;

typedef struct {
  CHAR8                   Identifier[16];
  EFI_PHYSICAL_ADDRESS    Base;
  UINT64                  Size;
} UNIVERSAL_PAYLOAD_EXTRA_DATA_ENTRY;

typedef struct {
  UNIVERSAL_PAYLOAD_GENERIC_HEADER      Header;
  UINT32                                Count;
  UNIVERSAL_PAYLOAD_EXTRA_DATA_ENTRY    Entry[0];
} UNIVERSAL_PAYLOAD_EXTRA_DATA;


RETURN_STATUS
BuildMemoryMap (
  EFI_MEMORY_DESCRIPTOR *Base,
  UINTN                 *Count
  );

void
LoadLinux (
  UINT32  BootParam,
  UINT32  KernelBase
  );
#endif