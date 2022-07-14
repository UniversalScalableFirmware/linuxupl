;------------------------------------------------------------------------------
;
; Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
; Portions copyright (c) 2011, Apple Inc. All rights reserved.<BR>
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
; Module Name:
;
;   InternalSwitchStack.nasm
;
; Abstract:
;
;   Implementation of a stack switch on IA-32.
;
;------------------------------------------------------------------------------

    SECTION .text

;------------------------------------------------------------------------------
; void
; EFIAPI
; InternalSwitchStack (
;      SWITCH_STACK_ENTRY_POINT  EntryPoint,
;      void                      *Context1,   
;      void                      *Context2,   
;      void                      *NewStack
;   );
;------------------------------------------------------------------------------
global InternalSwitchStack
InternalSwitchStack:
  push  ebp
  mov   ebp, esp

  mov   esp, [ebp + 20]    ; switch stack
  sub   esp, 8
  mov   eax, [ebp + 16]
  mov   [esp + 4], eax
  mov   eax, [ebp + 12]
  mov   [esp], eax
  push  0                  ; keeps gdb from unwinding stack
  jmp   dword [ebp + 8]    ; call and never return
