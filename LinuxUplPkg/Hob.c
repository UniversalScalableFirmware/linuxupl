/** @file

  Copyright (c) 2010, Apple Inc. All rights reserved.<BR>
  Copyright (c) 2017 - 2022, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "Base.h"

extern void *mHobList;


UINT64
ReadUnaligned64 (
   CONST UINT64              *Buffer
  )
{
  return *Buffer;
}

void *
GetHobList (
  void
  )
{
 return mHobList;
}

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
  )
{
  EFI_PEI_HOB_POINTERS  Hob;
  Hob.Raw = (UINT8 *)HobStart;
  //
  // Parse the HOB list until end of list or matching type is found.
  //
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == Type) {
      return Hob.Raw;
    }

    Hob.Raw = GET_NEXT_HOB (Hob);
  }
  return NULL;
}

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

unsigned char
CompareGuid (
    CONST GUID  *Guid1,
    CONST GUID  *Guid2
  )
{
  UINT64  LowPartOfGuid1;
  UINT64  LowPartOfGuid2;
  UINT64  HighPartOfGuid1;
  UINT64  HighPartOfGuid2;

  LowPartOfGuid1  = ReadUnaligned64 ((CONST UINT64 *)Guid1);
  LowPartOfGuid2  = ReadUnaligned64 ((CONST UINT64 *)Guid2);
  HighPartOfGuid1 = ReadUnaligned64 ((CONST UINT64 *)Guid1 + 1);
  HighPartOfGuid2 = ReadUnaligned64 ((CONST UINT64 *)Guid2 + 1);

  return (unsigned char)(LowPartOfGuid1 == LowPartOfGuid2 && HighPartOfGuid1 == HighPartOfGuid2);
}

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
  )
{
  EFI_PEI_HOB_POINTERS  GuidHob;
  
  GuidHob.Raw = (UINT8 *)HobStart;
  while ((GuidHob.Raw = GetNextHob (EFI_HOB_TYPE_GUID_EXTENSION, GuidHob.Raw)) != NULL) {
    if (CompareGuid (Guid, &GuidHob.Guid->Name)) {
      break;
    }

    GuidHob.Raw = GET_NEXT_HOB (GuidHob);
  }
  return GuidHob.Raw;
}

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
  )
{
  void  *HobList;
  HobList = GetHobList ();
  return GetNextGuidHob (Guid, HobList);
}
