/** @file
  GPL-2.0-only
**/

#include "LinuxPayloadEntry.h"
#include "Base.h"
void  *mHobList;

GUID gUniversalPayloadLinuxCommandLineGuid = {0xb5aeb34f, 0x3047, 0x4955, {0xb8, 0x80, 0xad, 0xd3, 0x6d, 0x86, 0xdc, 0x0f}};
GUID gUniversalPayloadExtraDataGuid = {0x15a5baf6, 0x1c91, 0x467d, {0x9d, 0xfb, 0x31, 0x9d, 0x17, 0x8d, 0x4b, 0xb4}};

/**
  Compares two Null-terminated ASCII strings, and returns the difference
  between the first mismatched ASCII characters.

  This function compares the Null-terminated ASCII string FirstString to the
  Null-terminated ASCII string SecondString. If FirstString is identical to
  SecondString, then 0 is returned. Otherwise, the value returned is the first
  mismatched ASCII character in SecondString subtracted from the first
  mismatched ASCII character in FirstString.

  If FirstString is NULL, then ASSERT().
  If SecondString is NULL, then ASSERT().
  If PcdMaximumAsciiStringLength is not zero and FirstString contains more than
  PcdMaximumAsciiStringLength ASCII characters, not including the Null-terminator,
  then ASSERT().
  If PcdMaximumAsciiStringLength is not zero and SecondString contains more
  than PcdMaximumAsciiStringLength ASCII characters, not including the
  Null-terminator, then ASSERT().

  @param  FirstString   A pointer to a Null-terminated ASCII string.
  @param  SecondString  A pointer to a Null-terminated ASCII string.

  @retval ==0      FirstString is identical to SecondString.
  @retval !=0      FirstString is not identical to SecondString.

**/
INTN
AsciiStrCmp (
     CONST CHAR8  *FirstString,
     CONST CHAR8  *SecondString
  )
{
  while ((*FirstString != '\0') && (*FirstString == *SecondString)) {
    FirstString++;
    SecondString++;
  }
  return *FirstString - *SecondString;
}

/**
  It will build boot params and boot to linux.

  @retval EFI_SUCCESS        If it completed successfully.
  @retval Others             If it failed to build required HOBs.
**/
EFI_STATUS
BuildBootParamsAndBootToLinux (
  void
  )
{
  UNIVERSAL_PAYLOAD_EXTRA_DATA          *ExtraData;
  UINT8                                 *GuidHob;
  struct linux_params                   *params;
  UINTN                                 IndexOfLinuxKernel;
  UINTN                                 IndexOfInitramfs;
  UINTN                                 IndexOfBootParams;
  UINTN                                 Index;
  EFI_MEMORY_DESCRIPTOR                 MemoryMap[E820MAX];
  UINTN                                 MemoryMapCount;
  MemoryMapCount = E820MAX;
  //
  // Get linux kernel, boot params and intiramfs
  //
  GuidHob = GetFirstGuidHob (&gUniversalPayloadExtraDataGuid);
  ExtraData          = (UNIVERSAL_PAYLOAD_EXTRA_DATA *)GET_GUID_HOB_DATA (GuidHob);
  IndexOfLinuxKernel = (UINTN)-1;
  IndexOfInitramfs   = (UINTN)-1;
  IndexOfBootParams  = (UINTN)-1;
  for (Index = 0; Index < ExtraData->Count; Index++) {
    if (AsciiStrCmp (ExtraData->Entry[Index].Identifier, "linux") == 0) {
      IndexOfLinuxKernel = Index;
    }
    if (AsciiStrCmp (ExtraData->Entry[Index].Identifier, "initramfs") == 0) {
      IndexOfInitramfs = Index;
    }
    if (AsciiStrCmp (ExtraData->Entry[Index].Identifier, "bootparams") == 0) {
      IndexOfBootParams = Index;
    }
  }

  params               = (void *)(UINTN)ExtraData->Entry[IndexOfBootParams].Base;
  params->kernel_start = (unsigned int)(UINTN)ExtraData->Entry[IndexOfLinuxKernel].Base;
  if (IndexOfInitramfs != (UINTN)-1) {
    params->initrd_start = ExtraData->Entry[IndexOfInitramfs].Base;
    params->initrd_size  = ExtraData->Entry[IndexOfInitramfs].Size;
  }
  //
  // Get command line.
  //
  params->cmd_line_ptr = (UINTN)params->command_line;

  //
  // Create memory map.
  //
  BuildMemoryMap (MemoryMap, &MemoryMapCount);

  //
  // Create E820 memory table
  //
  for (Index = 0; Index < MemoryMapCount; Index++) {
    params->e820_map[Index].addr = MemoryMap[Index].PhysicalStart;
    params->e820_map[Index].size = EFI_PAGES_TO_SIZE (MemoryMap[Index].NumberOfPages);
    params->e820_map[Index].type = MemoryMap[Index].Type;
    if (params->e820_map[Index].type == EfiConventionalMemory) {
      params->e820_map[Index].type = E820_RAM;
    } else {
      params->e820_map[Index].type = E820_RESERVED;
    }
  }

  params->e820_map_nr = MemoryMapCount;
  LoadLinux ((unsigned int)(void *)params, (unsigned int)(params->kernel_start));

  return EFI_SUCCESS;
}

/**
  It will find and switch to a new stack.

  @retval EFI_SUCCESS        If it completed successfully.
  @retval Others             If it failed to build required HOBs.
**/
EFI_STATUS
FindAndSwitchStack (
  void
  )
{
  EFI_PHYSICAL_ADDRESS          StackTop;
  UNIVERSAL_PAYLOAD_EXTRA_DATA  *ExtraData;
  UINT8                         *GuidHob;
  UINTN                         IndexOfStack;
  UINTN                         Index;
  //
  // Get a memory range for stack from extra data.
  //
  GuidHob = GetFirstGuidHob (&gUniversalPayloadExtraDataGuid);
  ExtraData    = (UNIVERSAL_PAYLOAD_EXTRA_DATA *)GET_GUID_HOB_DATA (GuidHob);
  IndexOfStack = (UINTN)-1;
  for (Index = 0; Index < ExtraData->Count; Index++) {
    if (AsciiStrCmp (ExtraData->Entry[Index].Identifier, "stack") == 0) {
      IndexOfStack = Index;
    }
  }

  StackTop = ExtraData->Entry[IndexOfStack].Base + ExtraData->Entry[IndexOfStack].Size;
  InternalSwitchStack (
    (SWITCH_STACK_ENTRY_POINT)(UINTN)BuildBootParamsAndBootToLinux,
    ((void *) 0),
    ((void *) 0),
    (void *)(UINTN)StackTop
    );
  return EFI_ABORTED;
}  
/**
  Entry point to the Linux Upl

  @param[in]   BootloaderParameter    The starting address of bootloader parameter block.

  @retval      It will not return if SUCCESS, and return error when passing bootloader parameter.
**/
EFI_STATUS
EFIAPI
_ModuleEntryPoint (
 UINTN  BootloaderParameter
  )
{
  mHobList = (void *)BootloaderParameter;
  FindAndSwitchStack ();
  return EFI_SUCCESS;
}
