/** @file
  GPL-2.0-only
**/

#include "LinuxPayloadEntry.h"

#define STACK_SIZE  0x20000
extern VOID  *mHobList;

/**
  It will build boot params and boot to linux.

  @retval EFI_SUCCESS        If it completed successfully.
  @retval Others             If it failed to build required HOBs.
**/
EFI_STATUS
BuildBootParamsAndBootToLinux (
  VOID
  )
{
  UNIVERSAL_PAYLOAD_EXTRA_DATA          *ExtraData;
  UINT8                                 *GuidHob;
  struct linux_params                   *params;
  UINTN                                 IndexOfLinuxKernel;
  UINTN                                 IndexOfInitramfs;
  UINTN                                 IndexOfBootParams;
  UINTN                                 Index;
  UNIVERSAL_PAYLOAD_LINUX_COMMAND_LINE  *LinuxCommandLineHob;
  EFI_MEMORY_DESCRIPTOR                 MemoryMap[E820MAX];
  UINTN                                 MemoryMapCount;

  MemoryMapCount = E820MAX;
  //
  // Get linux kernel, boot params and intiramfs
  //
  GuidHob = GetFirstGuidHob (&gUniversalPayloadExtraDataGuid);
  ASSERT (GuidHob != NULL);
  ExtraData          = (UNIVERSAL_PAYLOAD_EXTRA_DATA *)GET_GUID_HOB_DATA (GuidHob);
  IndexOfLinuxKernel = (UINTN)-1;
  IndexOfInitramfs   = (UINTN)-1;
  IndexOfBootParams  = (UINTN)-1;

  for (Index = 0; Index < ExtraData->Count; Index++) {
    if (AsciiStrCmp (ExtraData->Entry[Index].Identifier, "linux") == 0) {
      ASSERT (IndexOfLinuxKernel == (UINTN)-1);
      IndexOfLinuxKernel = Index;
    }

    if (AsciiStrCmp (ExtraData->Entry[Index].Identifier, "initramfs") == 0) {
      ASSERT (IndexOfInitramfs == (UINTN)-1);
      IndexOfInitramfs = Index;
    }

    if (AsciiStrCmp (ExtraData->Entry[Index].Identifier, "bootparams") == 0) {
      ASSERT (IndexOfBootParams == (UINTN)-1);
      IndexOfBootParams = Index;
    }
  }

  ASSERT (IndexOfLinuxKernel != (UINTN)-1);
  ASSERT (IndexOfBootParams != (UINTN)-1);

  params               = (VOID *)(UINTN)ExtraData->Entry[IndexOfBootParams].Base;
  params->kernel_start = (UINT32)(UINTN)ExtraData->Entry[IndexOfLinuxKernel].Base;
  DEBUG ((DEBUG_INFO, "params->kernel_start  = 0x%x\n", (UINTN)params->kernel_start));
  if (IndexOfInitramfs != (UINTN)-1) {
    params->initrd_start = ExtraData->Entry[IndexOfInitramfs].Base;
    params->initrd_size  = ExtraData->Entry[IndexOfInitramfs].Size;
  }

  //
  // Get command line.
  //
  GuidHob             = GetFirstGuidHob (&gUniversalPayloadLinuxCommandLineGuid);
  LinuxCommandLineHob = (UNIVERSAL_PAYLOAD_LINUX_COMMAND_LINE *)GET_GUID_HOB_DATA (GuidHob);
  DEBUG ((DEBUG_INFO, "LinuxCommandLineHob CommandLine = 0x%x\n", (UINTN)LinuxCommandLineHob->CommandLine));
  params->cmd_line_ptr = (UINTN)LinuxCommandLineHob->CommandLine;

  DEBUG ((DEBUG_INFO, "ExtraData->Entry[IndexOfLinuxKernel].Size = 0x%x\n", (UINTN)ExtraData->Entry[IndexOfLinuxKernel].Size));
  DEBUG ((DEBUG_INFO, "ExtraData->Entry[IndexOfLinuxKernel].Base = 0x%x\n", (UINTN)ExtraData->Entry[IndexOfLinuxKernel].Base));
  DEBUG ((DEBUG_INFO, "init_size = 0x%x\n", (UINTN)params->init_size));
  DEBUG ((DEBUG_INFO, "relocatable_kernel = 0x%x\n", (UINTN)params->relocatable_kernel));
  DEBUG ((DEBUG_INFO, "kernel_alignment  = 0x%x\n", (UINTN)params->kernel_alignment));

  //
  // Create memory map.
  //
  BuildMemoryMap (MemoryMap, &MemoryMapCount);
  DEBUG ((DEBUG_INFO, "MemoryMapCount  = %d\n", MemoryMapCount));

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

  DEBUG ((DEBUG_INFO, "LoadLinux = 0x%x\n", LoadLinux));
  LoadLinux ((UINT32)(VOID *)params, (UINT32)(params->kernel_start));

  return EFI_SUCCESS;
}

/**
  It will find and switch to a new stack.

  @retval EFI_SUCCESS        If it completed successfully.
  @retval Others             If it failed to build required HOBs.
**/
EFI_STATUS
FindAndSwitchStack (
  VOID
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
  ASSERT (GuidHob != NULL);
  ExtraData    = (UNIVERSAL_PAYLOAD_EXTRA_DATA *)GET_GUID_HOB_DATA (GuidHob);
  IndexOfStack = (UINTN)-1;
  for (Index = 0; Index < ExtraData->Count; Index++) {
    if (AsciiStrCmp (ExtraData->Entry[Index].Identifier, "stack") == 0) {
      ASSERT (IndexOfStack == (UINTN)-1);
      IndexOfStack = Index;
    }
  }

  ASSERT (IndexOfStack != (UINTN)-1);

  StackTop = ExtraData->Entry[IndexOfStack].Base + ExtraData->Entry[IndexOfStack].Size;
  DEBUG ((DEBUG_ERROR, "%a: %d switch stack to 0x%x\n", __FILE__, __LINE__, (UINTN)StackTop));
  SwitchStack (
    (SWITCH_STACK_ENTRY_POINT)(UINTN)BuildBootParamsAndBootToLinux,
    NULL,
    NULL,
    (VOID *)(UINTN)StackTop
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
  IN UINTN  BootloaderParameter
  )
{
  mHobList = (VOID *)BootloaderParameter;

  // Call constructor for all libraries
  ProcessLibraryConstructorList ();

  DEBUG ((DEBUG_INFO, "Entering Linux Universal Payload...\n"));
  DEBUG ((DEBUG_INFO, "Compiling time %a %a\n", __DATE__, __TIME__));
  DEBUG ((DEBUG_INFO, "sizeof(UINTN) = 0x%x\n", sizeof (UINTN)));

  DEBUG_CODE (
    //
    // Dump the Hobs from boot loader
    //
    PrintHob (mHobList);
    );

  // Build HOB based on information from Bootloader
  FindAndSwitchStack ();

  // Should not get here
  CpuDeadLoop ();
  return EFI_SUCCESS;
}
