/** @file

  Copyright (c) 2010, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2017 - 2022, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef __BASE_H__
#define __BASE_H__
#define EFIAPI  __attribute__((cdecl))

#define CONST  const
#define NULL  ((void *) 0)
#define TRUE  ((unsigned char)(1==1))
#define FALSE  ((unsigned char)(0==1))

typedef unsigned long long UINT64;

///
/// 8-byte signed value.
///
typedef long long INT64;

///
/// 4-byte unsigned value.
///
typedef unsigned int UINT32;

///
/// 4-byte signed value.
///
typedef int INT32;

///
/// 2-byte unsigned value.
///
typedef unsigned short UINT16;

///
/// 2-byte Character.  Unless otherwise specified all strings are stored in the
/// UTF-16 encoding format as defined by Unicode 2.1 and ISO/IEC 10646 standards.
///
typedef unsigned short CHAR16;

///
/// 2-byte signed value.
///
typedef short INT16;

typedef unsigned char BOOLEAN;
///
/// 1-byte unsigned value.
///
typedef unsigned char UINT8;

///
/// 1-byte Character
///
typedef char CHAR8;

///
/// 1-byte signed value
///
typedef signed char INT8;

///
/// Unsigned value of native width.  (4 bytes on supported 32-bit processor instructions;
/// 8 bytes on supported 64-bit processor instructions.)
///
typedef UINT32 UINTN;

///
/// Signed value of native width.  (4 bytes on supported 32-bit processor instructions;
/// 8 bytes on supported 64-bit processor instructions.)
///
typedef INT32 INTN;

typedef UINT64 PHYSICAL_ADDRESS;

///
/// Function return status for EFI API.
///
typedef UINTN RETURN_STATUS;

typedef RETURN_STATUS EFI_STATUS;

///
/// 64-bit physical memory address.
///
typedef UINT64 EFI_PHYSICAL_ADDRESS;

///
/// 64-bit virtual memory address.
///
typedef UINT64 EFI_VIRTUAL_ADDRESS;

typedef struct {
    UINT32    Data1;
    UINT16    Data2;
    UINT16    Data3;
    UINT8     Data4[8];
} GUID;

typedef GUID EFI_GUID;

///
/// Describes the format and size of the data inside the HOB.
/// All HOBs must contain this generic HOB header.
///
typedef struct {
  ///
  /// Identifies the HOB data structure type.
  ///
  UINT16    HobType;
  ///
  /// The length in bytes of the HOB.
  ///
  UINT16    HobLength;
  ///
  /// This field must always be set to zero.
  ///
  unsigned int    Reserved;
} EFI_HOB_GENERIC_HEADER;

///
/// Allows writers of executable content in the HOB producer phase to
/// maintain and manage HOBs with specific GUID.
///
typedef struct {
  ///
  /// The HOB generic header. Header.HobType = EFI_HOB_TYPE_GUID_EXTENSION.
  ///
  EFI_HOB_GENERIC_HEADER    Header;
  ///
  /// A GUID that defines the contents of this HOB.
  ///
  EFI_GUID                  Name;
  //
  // Guid specific data goes here
  //
} EFI_HOB_GUID_TYPE;

typedef unsigned int EFI_RESOURCE_TYPE;
#define EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE              0x00000400
#define EFI_RESOURCE_ATTRIBUTE_UNCACHED_EXPORTED        0x00020000
#define EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE        0x00000800
#define EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE  0x00001000
#define EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE     0x00002000
#define EFI_RESOURCE_ATTRIBUTE_WRITE_PROTECTABLE        0x00200000
#define EFI_RESOURCE_ATTRIBUTE_EXECUTION_PROTECTABLE    0x00400000
#define EFI_RESOURCE_ATTRIBUTE_PERSISTABLE              0x01000000
#define EFI_RESOURCE_ATTRIBUTE_MORE_RELIABLE            0x02000000
#define EFI_RESOURCE_ATTRIBUTE_READ_PROTECTABLE         0x00100000
#define EFI_RESOURCE_ATTRIBUTE_READ_ONLY_PROTECTABLE    0x00080000
#define EFI_RESOURCE_SYSTEM_MEMORY                      0x00000000
#define EFI_RESOURCE_MEMORY_RESERVED                    0x00000005

typedef enum {
  EfiReservedMemoryType,
  EfiLoaderCode,
  EfiLoaderData,
  EfiBootServicesCode,
  EfiBootServicesData,
  EfiRuntimeServicesCode,
  EfiRuntimeServicesData,
  EfiConventionalMemory,
  EfiUnusableMemory,
  EfiACPIReclaimMemory,
  EfiACPIMemoryNVS,
  EfiMemoryMappedIO,
  EfiMemoryMappedIOPortSpace,
  EfiPalCode,
  EfiPersistentMemory,
  EfiMaxMemoryType
} EFI_MEMORY_TYPE;

typedef struct {
  ///
  /// A GUID that defines the memory allocation region's type and purpose, as well as
  /// other fields within the memory allocation HOB. This GUID is used to define the
  /// additional data within the HOB that may be present for the memory allocation HOB.
  /// Type EFI_GUID is defined in InstallProtocolInterface() in the UEFI 2.0
  /// specification.
  ///
  EFI_GUID                Name;

  ///
  /// The base address of memory allocated by this HOB. Type
  /// EFI_PHYSICAL_ADDRESS is defined in AllocatePages() in the UEFI 2.0
  /// specification.
  ///
  EFI_PHYSICAL_ADDRESS    MemoryBaseAddress;

  ///
  /// The length in bytes of memory allocated by this HOB.
  ///
  UINT64                  MemoryLength;

  ///
  /// Defines the type of memory allocated by this HOB. The memory type definition
  /// follows the EFI_MEMORY_TYPE definition. Type EFI_MEMORY_TYPE is defined
  /// in AllocatePages() in the UEFI 2.0 specification.
  ///
  EFI_MEMORY_TYPE         MemoryType;

  ///
  /// Padding for Itanium processor family
  ///
  UINT8                   Reserved[4];
} EFI_HOB_MEMORY_ALLOCATION_HEADER;

typedef struct {
  ///
  /// The HOB generic header. Header.HobType = EFI_HOB_TYPE_MEMORY_ALLOCATION.
  ///
  EFI_HOB_GENERIC_HEADER              Header;
  ///
  /// An instance of the EFI_HOB_MEMORY_ALLOCATION_HEADER that describes the
  /// various attributes of the logical memory allocation.
  ///
  EFI_HOB_MEMORY_ALLOCATION_HEADER    AllocDescriptor;
  //
  // Additional data pertaining to the "Name" Guid memory
  // may go here.
  //
} EFI_HOB_MEMORY_ALLOCATION;

#define MAX_BIT      0x8000000000000000
//
// Return the maximum of two operands.
// This macro returns the maximum of two operand specified by a and b.
// Both a and b must be the same numerical types, signed or unsigned.
//
#define MAX(a, b)                       \
  (((a) > (b)) ? (a) : (b))

//
// Return the minimum of two operands.
// This macro returns the minimal of two operand specified by a and b.
// Both a and b must be the same numerical types, signed or unsigned.
//
#define MIN(a, b)                       \
  (((a) < (b)) ? (a) : (b))

//
// EFI Error Codes common to all execution phases
//
typedef UINTN RETURN_STATUS;

///
/// Set the upper bit to indicate EFI Error.
///
#define ENCODE_ERROR(a)              ((RETURN_STATUS)(MAX_BIT | (a)))
#define ENCODE_WARNING(a)            ((RETURN_STATUS)(a))
#define RETURN_ERROR(a)              (((INTN)(RETURN_STATUS)(a)) < 0)
#define RETURN_ABORTED               ENCODE_ERROR (21)
#define RETURN_UNSUPPORTED           ENCODE_ERROR (3)
#define RETURN_NOT_FOUND             ENCODE_ERROR (14)
#define RETURN_SUCCESS               0
#define EFI_SUCCESS                  RETURN_SUCCESS
#define EFI_ABORTED               	 RETURN_ABORTED
#define EFI_NOT_FOUND                RETURN_NOT_FOUND
//
// The EFI memory allocation functions work in units of EFI_PAGEs that are
// 4KB. This should in no way be confused with the page size of the processor.
// An EFI_PAGE is just the quanta of memory in EFI.
//
#define SIZE_4KB    0x00001000
#define EFI_PAGE_SIZE   SIZE_4KB
#define EFI_PAGE_MASK   0xFFF
#define EFI_PAGE_SHIFT  12
#define EFI_HOB_TYPE_END_OF_HOB_LIST      0xFFFF
#define EFI_HOB_TYPE_UNUSED               0xFFFE
#define EFI_HOB_TYPE_MEMORY_ALLOCATION    0x0002
#define EFI_HOB_TYPE_RESOURCE_DESCRIPTOR  0x0003
#define EFI_HOB_TYPE_GUID_EXTENSION       0x0004
/**
  Macro that converts a size, in bytes, to a number of EFI_PAGESs.

  @param  Size      A size in bytes.  This parameter is assumed to be type UINTN.
                    Passing in a parameter that is larger than UINTN may produce
                    unexpected results.

  @return  The number of EFI_PAGESs associated with the number of bytes specified
           by Size.

**/
#define EFI_SIZE_TO_PAGES(Size)  (((Size) >> EFI_PAGE_SHIFT) + (((Size) & EFI_PAGE_MASK) ? 1 : 0))

/**
  Macro that converts a number of EFI_PAGEs to a size in bytes.

  @param  Pages     The number of EFI_PAGES.  This parameter is assumed to be
                    type UINTN.  Passing in a parameter that is larger than
                    UINTN may produce unexpected results.

  @return  The number of bytes associated with the number of EFI_PAGEs specified
           by Pages.

**/
#define EFI_PAGES_TO_SIZE(Pages)  ((Pages) << EFI_PAGE_SHIFT)

typedef struct {
  ///
  /// The HOB generic header. Header.HobType = EFI_HOB_TYPE_MEMORY_ALLOCATION.
  ///
  EFI_HOB_GENERIC_HEADER              Header;
  ///
  /// An instance of the EFI_HOB_MEMORY_ALLOCATION_HEADER that describes the
  /// various attributes of the logical memory allocation.
  ///
  EFI_HOB_MEMORY_ALLOCATION_HEADER    AllocDescriptor;
} EFI_HOB_MEMORY_ALLOCATION_BSP_STORE;

typedef struct {
  ///
  /// The HOB generic header. Header.HobType = EFI_HOB_TYPE_MEMORY_ALLOCATION.
  ///
  EFI_HOB_GENERIC_HEADER              Header;
  ///
  /// An instance of the EFI_HOB_MEMORY_ALLOCATION_HEADER that describes the
  /// various attributes of the logical memory allocation.
  ///
  EFI_HOB_MEMORY_ALLOCATION_HEADER    AllocDescriptor;
} EFI_HOB_MEMORY_ALLOCATION_STACK;

typedef unsigned int EFI_BOOT_MODE;

typedef unsigned int EFI_RESOURCE_ATTRIBUTE_TYPE;

typedef struct {
  ///
  /// The HOB generic header. Header.HobType = EFI_HOB_TYPE_HANDOFF.
  ///
  EFI_HOB_GENERIC_HEADER    Header;
  ///
  /// The version number pertaining to the PHIT HOB definition.
  /// This value is four bytes in length to provide an 8-byte aligned entry
  /// when it is combined with the 4-byte BootMode.
  ///
  unsigned int                    Version;
  ///
  /// The system boot mode as determined during the HOB producer phase.
  ///
  EFI_BOOT_MODE             BootMode;
  ///
  /// The highest address location of memory that is allocated for use by the HOB producer
  /// phase. This address must be 4-KB aligned to meet page restrictions of UEFI.
  ///
  EFI_PHYSICAL_ADDRESS      EfiMemoryTop;
  ///
  /// The lowest address location of memory that is allocated for use by the HOB producer phase.
  ///
  EFI_PHYSICAL_ADDRESS      EfiMemoryBottom;
  ///
  /// The highest address location of free memory that is currently available
  /// for use by the HOB producer phase.
  ///
  EFI_PHYSICAL_ADDRESS      EfiFreeMemoryTop;
  ///
  /// The lowest address location of free memory that is available for use by the HOB producer phase.
  ///
  EFI_PHYSICAL_ADDRESS      EfiFreeMemoryBottom;
  ///
  /// The end of the HOB list.
  ///
  EFI_PHYSICAL_ADDRESS      EfiEndOfHobList;
} EFI_HOB_HANDOFF_INFO_TABLE;

typedef struct {
  ///
  /// Type of the memory region.
  /// Type EFI_MEMORY_TYPE is defined in the
  /// AllocatePages() function description.
  ///
  unsigned int                  Type;
  ///
  /// Physical address of the first byte in the memory region. PhysicalStart must be
  /// aligned on a 4 KiB boundary, and must not be above 0xfffffffffffff000. Type
  /// EFI_PHYSICAL_ADDRESS is defined in the AllocatePages() function description
  ///
  EFI_PHYSICAL_ADDRESS    PhysicalStart;
  ///
  /// Virtual address of the first byte in the memory region.
  /// VirtualStart must be aligned on a 4 KiB boundary,
  /// and must not be above 0xfffffffffffff000.
  ///
  EFI_VIRTUAL_ADDRESS     VirtualStart;
  ///
  /// NumberOfPagesNumber of 4 KiB pages in the memory region.
  /// NumberOfPages must not be 0, and must not be any value
  /// that would represent a memory page with a start address,
  /// either physical or virtual, above 0xfffffffffffff000.
  ///
  UINT64                  NumberOfPages;
  ///
  /// Attributes of the memory region that describe the bit mask of capabilities
  /// for that memory region, and not necessarily the current settings for that
  /// memory region.
  ///
  UINT64                  Attribute;
} EFI_MEMORY_DESCRIPTOR;

typedef struct {
  ///
  /// The HOB generic header. Header.HobType = EFI_HOB_TYPE_MEMORY_ALLOCATION.
  ///
  EFI_HOB_GENERIC_HEADER              Header;
  ///
  /// An instance of the EFI_HOB_MEMORY_ALLOCATION_HEADER that describes the
  /// various attributes of the logical memory allocation.
  ///
  EFI_HOB_MEMORY_ALLOCATION_HEADER    MemoryAllocationHeader;
  ///
  /// The GUID specifying the values of the firmware file system name
  /// that contains the HOB consumer phase component.
  ///
  EFI_GUID                            ModuleName;
  ///
  /// The address of the memory-mapped firmware volume
  /// that contains the HOB consumer phase firmware file.
  ///
  EFI_PHYSICAL_ADDRESS                EntryPoint;
} EFI_HOB_MEMORY_ALLOCATION_MODULE;

typedef struct {
  ///
  /// The HOB generic header. Header.HobType = EFI_HOB_TYPE_RESOURCE_DESCRIPTOR.
  ///
  EFI_HOB_GENERIC_HEADER         Header;
  ///
  /// A GUID representing the owner of the resource. This GUID is used by HOB
  /// consumer phase components to correlate device ownership of a resource.
  ///
  EFI_GUID                       Owner;
  ///
  /// The resource type enumeration as defined by EFI_RESOURCE_TYPE.
  ///
  EFI_RESOURCE_TYPE              ResourceType;
  ///
  /// Resource attributes as defined by EFI_RESOURCE_ATTRIBUTE_TYPE.
  ///
  EFI_RESOURCE_ATTRIBUTE_TYPE    ResourceAttribute;
  ///
  /// The physical start address of the resource region.
  ///
  EFI_PHYSICAL_ADDRESS           PhysicalStart;
  ///
  /// The number of bytes of the resource region.
  ///
  UINT64                         ResourceLength;
} EFI_HOB_RESOURCE_DESCRIPTOR;

typedef struct {
  ///
  /// The HOB generic header. Header.HobType = EFI_HOB_TYPE_FV.
  ///
  EFI_HOB_GENERIC_HEADER    Header;
  ///
  /// The physical memory-mapped base address of the firmware volume.
  ///
  EFI_PHYSICAL_ADDRESS      BaseAddress;
  ///
  /// The length in bytes of the firmware volume.
  ///
  UINT64                    Length;
} EFI_HOB_FIRMWARE_VOLUME;

typedef struct {
  ///
  /// The HOB generic header. Header.HobType = EFI_HOB_TYPE_FV2.
  ///
  EFI_HOB_GENERIC_HEADER    Header;
  ///
  /// The physical memory-mapped base address of the firmware volume.
  ///
  EFI_PHYSICAL_ADDRESS      BaseAddress;
  ///
  /// The length in bytes of the firmware volume.
  ///
  UINT64                    Length;
  ///
  /// The name of the firmware volume.
  ///
  EFI_GUID                  FvName;
  ///
  /// The name of the firmware file that contained this firmware volume.
  ///
  EFI_GUID                  FileName;
} EFI_HOB_FIRMWARE_VOLUME2;

typedef struct {
  ///
  /// The HOB generic header. Header.HobType = EFI_HOB_TYPE_FV3.
  ///
  EFI_HOB_GENERIC_HEADER    Header;
  ///
  /// The physical memory-mapped base address of the firmware volume.
  ///
  EFI_PHYSICAL_ADDRESS      BaseAddress;
  ///
  /// The length in bytes of the firmware volume.
  ///
  UINT64                    Length;
  ///
  /// The authentication status.
  ///
  unsigned int                    AuthenticationStatus;
  ///
  /// TRUE if the FV was extracted as a file within another firmware volume.
  /// FALSE otherwise.
  ///
  unsigned char                   ExtractedFv;
  ///
  /// The name of the firmware volume.
  /// Valid only if IsExtractedFv is TRUE.
  ///
  EFI_GUID                  FvName;
  ///
  /// The name of the firmware file that contained this firmware volume.
  /// Valid only if IsExtractedFv is TRUE.
  ///
  EFI_GUID                  FileName;
} EFI_HOB_FIRMWARE_VOLUME3;

typedef struct {
  ///
  /// The HOB generic header. Header.HobType = EFI_HOB_TYPE_CPU.
  ///
  EFI_HOB_GENERIC_HEADER    Header;
  ///
  /// Identifies the maximum physical memory addressability of the processor.
  ///
  UINT8                     SizeOfMemorySpace;
  ///
  /// Identifies the maximum physical I/O addressability of the processor.
  ///
  UINT8                     SizeOfIoSpace;
  ///
  /// This field will always be set to zero.
  ///
  UINT8                     Reserved[6];
} EFI_HOB_CPU;

typedef struct {
  ///
  /// The HOB generic header. Header.HobType = EFI_HOB_TYPE_MEMORY_POOL.
  ///
  EFI_HOB_GENERIC_HEADER    Header;
} EFI_HOB_MEMORY_POOL;

typedef struct {
  ///
  /// The HOB generic header where Header.HobType = EFI_HOB_TYPE_UEFI_CAPSULE.
  ///
  EFI_HOB_GENERIC_HEADER    Header;

  ///
  /// The physical memory-mapped base address of an UEFI capsule. This value is set to
  /// point to the base of the contiguous memory of the UEFI capsule.
  /// The length of the contiguous memory in bytes.
  ///
  EFI_PHYSICAL_ADDRESS      BaseAddress;
  UINT64                    Length;
} EFI_HOB_UEFI_CAPSULE;

typedef union {
  EFI_HOB_GENERIC_HEADER                 *Header;
  EFI_HOB_HANDOFF_INFO_TABLE             *HandoffInformationTable;
  EFI_HOB_MEMORY_ALLOCATION              *MemoryAllocation;
  EFI_HOB_MEMORY_ALLOCATION_BSP_STORE    *MemoryAllocationBspStore;
  EFI_HOB_MEMORY_ALLOCATION_STACK        *MemoryAllocationStack;
  EFI_HOB_MEMORY_ALLOCATION_MODULE       *MemoryAllocationModule;
  EFI_HOB_RESOURCE_DESCRIPTOR            *ResourceDescriptor;
  EFI_HOB_GUID_TYPE                      *Guid;
  EFI_HOB_FIRMWARE_VOLUME                *FirmwareVolume;
  EFI_HOB_FIRMWARE_VOLUME2               *FirmwareVolume2;
  EFI_HOB_FIRMWARE_VOLUME3               *FirmwareVolume3;
  EFI_HOB_CPU                            *Cpu;
  EFI_HOB_MEMORY_POOL                    *Pool;
  EFI_HOB_UEFI_CAPSULE                   *Capsule;
  UINT8                                  *Raw;
} EFI_PEI_HOB_POINTERS;
typedef
void
( *SWITCH_STACK_ENTRY_POINT)(
   void                      *Context1   ,
   void                      *Context2   
  );

void
InternalSwitchStack (
     SWITCH_STACK_ENTRY_POINT  EntryPoint,
     void                      *Context1    ,
     void                      *Context2    ,
     void                      *NewStack
  );  

typedef
INTN
( *BASE_SORT_COMPARE)(
    CONST void                 *Buffer1,
    CONST void                 *Buffer2
  );

void
QuickSort (
  void                 *BufferToSort,
    CONST UINTN              Count,
    CONST UINTN              ElementSize,
    BASE_SORT_COMPARE  CompareFunction,
  void                    *BufferOneElement
  );

#define GET_GUID_HOB_DATA(HobStart) \
  (void *)(*(UINT8 **)&(HobStart) + sizeof (EFI_HOB_GUID_TYPE))

#define END_OF_HOB_LIST(HobStart)  (GET_HOB_TYPE (HobStart) == (UINT16)EFI_HOB_TYPE_END_OF_HOB_LIST)

#define GET_HOB_TYPE(HobStart) \
  ((*(EFI_HOB_GENERIC_HEADER **)&(HobStart))->HobType)

#define GET_HOB_LENGTH(HobStart) \
  ((*(EFI_HOB_GENERIC_HEADER **)&(HobStart))->HobLength)

#define GET_NEXT_HOB(HobStart) \
  (void *)(*(UINT8 **)&(HobStart) + GET_HOB_LENGTH (HobStart))

/**
  Returns the pointer to the HOB list.

  This function returns the pointer to first HOB in the list.

  @return The pointer to the HOB list.

**/
void *
GetHobList (
  void
  );

/**
  Returns the next instance of a HOB type from the starting HOB.

  This function searches the first instance of a HOB type from the starting HOB pointer.
  If there does not exist such HOB type from the starting HOB pointer, it will return NULL.
  In contrast with macro GET_NEXT_HOB(), this function does not skip the starting HOB pointer
  unconditionally: it returns HobStart back if HobStart itself meets the requirement;
  caller is required to use GET_NEXT_HOB() if it wishes to skip current HobStart.
  If HobStart is NULL, then ASSERT().

  @param  Type          The HOB type to return.
  @param  HobStart      The starting HOB pointer to search from.

  @return The next instance of a HOB type from the starting HOB.

**/
void *
GetNextHob (
 UINT16      Type,
    CONST void  *HobStart
  );

/**
  This function searches the first instance of a HOB from the starting HOB pointer.
  Such HOB should satisfy two conditions:
  its HOB type is EFI_HOB_TYPE_GUID_EXTENSION and its GUID Name equals to the input Guid.
  If there does not exist such HOB from the starting HOB pointer, it will return NULL.
  Caller is required to apply GET_GUID_HOB_DATA () and GET_GUID_HOB_DATA_SIZE ()
  to extract the data section and its size info respectively.
  In contrast with macro GET_NEXT_HOB(), this function does not skip the starting HOB pointer
  unconditionally: it returns HobStart back if HobStart itself meets the requirement;
  caller is required to use GET_NEXT_HOB() if it wishes to skip current HobStart.
  If Guid is NULL, then ASSERT().
  If HobStart is NULL, then ASSERT().

  @param  Guid          The GUID to match with in the HOB list.
  @param  HobStart      A pointer to a Guid.

  @return The next instance of the matched GUID HOB from the starting HOB.

**/
void *
GetNextGuidHob (
    CONST EFI_GUID  *Guid,
    CONST void      *HobStart
  );

/**
  This function searches the first instance of a HOB among the whole HOB list.
  Such HOB should satisfy two conditions:
  its HOB type is EFI_HOB_TYPE_GUID_EXTENSION and its GUID Name equals to the input Guid.
  If there does not exist such HOB from the starting HOB pointer, it will return NULL.
  Caller is required to apply GET_GUID_HOB_DATA () and GET_GUID_HOB_DATA_SIZE ()
  to extract the data section and its size info respectively.
  If Guid is NULL, then ASSERT().

  @param  Guid          The GUID to match with in the HOB list.

  @return The first instance of the matched GUID HOB among the whole HOB list.

**/
void *
GetFirstGuidHob (
    CONST EFI_GUID  *Guid
  );

/**
  Compares two GUIDs.

  This function compares Guid1 to Guid2.  If the GUIDs are identical then TRUE is returned.
  If there are any bit differences in the two GUIDs, then FALSE is returned.

  If Guid1 is NULL, then ASSERT().
  If Guid2 is NULL, then ASSERT().

  @param  Guid1       A pointer to a 128 bit GUID.
  @param  Guid2       A pointer to a 128 bit GUID.

  @retval TRUE        Guid1 and Guid2 are identical.
  @retval FALSE       Guid1 and Guid2 are not identical.

**/
BOOLEAN
CompareGuid (
    CONST GUID  *Guid1,
    CONST GUID  *Guid2
  );
#endif