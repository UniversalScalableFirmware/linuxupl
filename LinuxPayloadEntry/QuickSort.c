/** @file
  Math worker functions.

  Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Base.h"

typedef
INTN
( *BASE_SORT_COMPARE)(
    CONST void                 *Buffer1,
    CONST void                 *Buffer2
  );

/**
  Copy Length bytes from Source to Destination.

  @param  DestinationBuffer The target of the copy request.
  @param  SourceBuffer      The place to copy from.
  @param  Length            The number of bytes to copy.

  @return Destination

**/

void *
InternalMemCopyMem (
      void        *DestinationBuffer,
  CONST void  *SourceBuffer,
  UINTN       Length
  )
{
  //
  // Declare the local variables that actually move the data elements as
  // volatile to prevent the optimizer from replacing this function with
  // the intrinsic memcpy()
  //
  volatile UINT8   *Destination8;
  CONST UINT8      *Source8;
  volatile unsigned int  *Destination32;
  CONST unsigned int     *Source32;
  volatile UINT64  *Destination64;
  CONST UINT64     *Source64;
  UINTN            Alignment;

  if ((((UINTN)DestinationBuffer & 0x7) == 0) && (((UINTN)SourceBuffer & 0x7) == 0) && (Length >= 8)) {
    if (SourceBuffer > DestinationBuffer) {
      Destination64 = (UINT64 *)DestinationBuffer;
      Source64      = (CONST UINT64 *)SourceBuffer;
      while (Length >= 8) {
        *(Destination64++) = *(Source64++);
        Length            -= 8;
      }

      // Finish if there are still some bytes to copy
      Destination8 = (UINT8 *)Destination64;
      Source8      = (CONST UINT8 *)Source64;
      while (Length-- != 0) {
        *(Destination8++) = *(Source8++);
      }
    } else if (SourceBuffer < DestinationBuffer) {
      Destination64 = (UINT64 *)((UINTN)DestinationBuffer + Length);
      Source64      = (CONST UINT64 *)((UINTN)SourceBuffer + Length);

      // Destination64 and Source64 were aligned on a 64-bit boundary
      // but if length is not a multiple of 8 bytes then they won't be
      // anymore.

      Alignment = Length & 0x7;
      if (Alignment != 0) {
        Destination8 = (UINT8 *)Destination64;
        Source8      = (CONST UINT8 *)Source64;

        while (Alignment-- != 0) {
          *(--Destination8) = *(--Source8);
          --Length;
        }

        Destination64 = (UINT64 *)Destination8;
        Source64      = (CONST UINT64 *)Source8;
      }

      while (Length > 0) {
        *(--Destination64) = *(--Source64);
        Length            -= 8;
      }
    }
  } else if ((((UINTN)DestinationBuffer & 0x3) == 0) && (((UINTN)SourceBuffer & 0x3) == 0) && (Length >= 4)) {
    if (SourceBuffer > DestinationBuffer) {
      Destination32 = (unsigned int *)DestinationBuffer;
      Source32      = (CONST unsigned int *)SourceBuffer;
      while (Length >= 4) {
        *(Destination32++) = *(Source32++);
        Length            -= 4;
      }

      // Finish if there are still some bytes to copy
      Destination8 = (UINT8 *)Destination32;
      Source8      = (CONST UINT8 *)Source32;
      while (Length-- != 0) {
        *(Destination8++) = *(Source8++);
      }
    } else if (SourceBuffer < DestinationBuffer) {
      Destination32 = (unsigned int *)((UINTN)DestinationBuffer + Length);
      Source32      = (CONST unsigned int *)((UINTN)SourceBuffer + Length);

      // Destination32 and Source32 were aligned on a 32-bit boundary
      // but if length is not a multiple of 4 bytes then they won't be
      // anymore.

      Alignment = Length & 0x3;
      if (Alignment != 0) {
        Destination8 = (UINT8 *)Destination32;
        Source8      = (CONST UINT8 *)Source32;

        while (Alignment-- != 0) {
          *(--Destination8) = *(--Source8);
          --Length;
        }

        Destination32 = (unsigned int *)Destination8;
        Source32      = (CONST unsigned int *)Source8;
      }

      while (Length > 0) {
        *(--Destination32) = *(--Source32);
        Length            -= 4;
      }
    }
  } else {
    if (SourceBuffer > DestinationBuffer) {
      Destination8 = (UINT8 *)DestinationBuffer;
      Source8      = (CONST UINT8 *)SourceBuffer;
      while (Length-- != 0) {
        *(Destination8++) = *(Source8++);
      }
    } else if (SourceBuffer < DestinationBuffer) {
      Destination8 = (UINT8 *)DestinationBuffer + (Length - 1);
      Source8      = (CONST UINT8 *)SourceBuffer + (Length - 1);
      while (Length-- != 0) {
        *(Destination8--) = *(Source8--);
      }
    }
  }

  return DestinationBuffer;
}
/**
  Copies a source buffer to a destination buffer, and returns the destination buffer.

  This function copies Length bytes from SourceBuffer to DestinationBuffer, and returns
  DestinationBuffer.  The implementation must be reentrant, and it must handle the case
  where SourceBuffer overlaps DestinationBuffer.

  If Length is greater than (MAX_ADDRESS - DestinationBuffer + 1), then ASSERT().
  If Length is greater than (MAX_ADDRESS - SourceBuffer + 1), then ASSERT().

  @param  DestinationBuffer   The pointer to the destination buffer of the memory copy.
  @param  SourceBuffer        The pointer to the source buffer of the memory copy.
  @param  Length              The number of bytes to copy from SourceBuffer to DestinationBuffer.

  @return DestinationBuffer.

**/
void *
CopyMem (
  void       *DestinationBuffer,
    CONST void  *SourceBuffer,
 UINTN       Length
  )
{
  if (Length == 0) {
    return DestinationBuffer;
  }

  if (DestinationBuffer == SourceBuffer) {
    return DestinationBuffer;
  }

  return InternalMemCopyMem (DestinationBuffer, SourceBuffer, Length);
}


/**
  This function is identical to perform QuickSort,
  except that is uses the pre-allocated buffer so the in place sorting does not need to
  allocate and free buffers constantly.

  Each element must be equal sized.

  if BufferToSort is NULL, then ASSERT.
  if CompareFunction is NULL, then ASSERT.
  if BufferOneElement is NULL, then ASSERT.
  if ElementSize is < 1, then ASSERT.

  if Count is < 2 then perform no action.

  @param[in, out] BufferToSort   on call a Buffer of (possibly sorted) elements
                                 on return a buffer of sorted elements
  @param[in] Count               the number of elements in the buffer to sort
  @param[in] ElementSize         Size of an element in bytes
  @param[in] CompareFunction     The function to call to perform the comparison
                                 of any 2 elements
  @param[out] BufferOneElement   Caller provided buffer whose size equals to ElementSize.
                                 It's used by QuickSort() for swapping in sorting.
**/
void
QuickSort (
  void                 *BufferToSort,
    CONST UINTN              Count,
    CONST UINTN              ElementSize,
    BASE_SORT_COMPARE  CompareFunction,
  void                    *BufferOneElement
  )
{
  void   *Pivot;
  UINTN  LoopCount;
  UINTN  NextSwapLocation;
  
  if (Count < 2) {
    return;
  }

  NextSwapLocation = 0;

  //
  // pick a pivot (we choose last element)
  //
  Pivot = ((UINT8 *)BufferToSort + ((Count - 1) * ElementSize));

  //
  // Now get the pivot such that all on "left" are below it
  // and everything "right" are above it
  //
  for (LoopCount = 0; LoopCount < Count -1; LoopCount++) {
    //
    // if the element is less than or equal to the pivot
    //
    if (CompareFunction (( void *)((UINT8 *)BufferToSort + ((LoopCount) * ElementSize)), Pivot) <= 0) {
      //
      // swap
      //
      CopyMem (BufferOneElement, (UINT8 *)BufferToSort + (NextSwapLocation * ElementSize), ElementSize);
      CopyMem ((UINT8 *)BufferToSort + (NextSwapLocation * ElementSize), (UINT8 *)BufferToSort + ((LoopCount) * ElementSize), ElementSize);
      CopyMem ((UINT8 *)BufferToSort + ((LoopCount)*ElementSize), BufferOneElement, ElementSize);

      //
      // increment NextSwapLocation
      //
      NextSwapLocation++;
    }
  }

  //
  // swap pivot to it's final position (NextSwapLocation)
  //
  CopyMem (BufferOneElement, Pivot, ElementSize);
  CopyMem (Pivot, (UINT8 *)BufferToSort + (NextSwapLocation * ElementSize), ElementSize);
  CopyMem ((UINT8 *)BufferToSort + (NextSwapLocation * ElementSize), BufferOneElement, ElementSize);

  //
  // Now recurse on 2 partial lists.  neither of these will have the 'pivot' element
  // IE list is sorted left half, pivot element, sorted right half...
  //
  if (NextSwapLocation >= 2) {
    QuickSort (
      BufferToSort,
      NextSwapLocation,
      ElementSize,
      CompareFunction,
      BufferOneElement
      );
  }

  if ((Count - NextSwapLocation - 1) >= 2) {
    QuickSort (
      (UINT8 *)BufferToSort + (NextSwapLocation + 1) * ElementSize,
      Count - NextSwapLocation - 1,
      ElementSize,
      CompareFunction,
      BufferOneElement
      );
  }
}
