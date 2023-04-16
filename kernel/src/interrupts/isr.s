.text

# We can't call lidt from c, so we have to drop to assembly to select the
# interrupt table
.global load_idt
load_idt:
  push %ebp
  mov %esp, %ebp

  mov 8(%esp), %eax
  lidt (%eax)

  mov %ebp, %esp
  pop %ebp

  ret

.macro isr vec, name
.global \name
\name:
  # Preserve the state of the machine before we jump to the isr
  pusha

  # Do the call
  mov $\vec, %eax
  push %eax
  call globalISR
  pop %eax

  # Return
  popa
  iret
.endm


ISR 0,divideByZero_ISR
ISR 1,debug_ISR
ISR 2,NMI_ISR
ISR 3,breakpoint_ISR
ISR 4,overflow_ISR
ISR 5,boundExceeded_ISR
ISR 6,invalidOpcode_ISR
ISR 7,deviceNotAvailable_ISR
ISR 9,coprocessorSegmentOverrun_ISR
ISR 10,invalidTSS_ISR
ISR 11,segmentNotPresent_ISR
ISR 15,stackFault_ISR
ISR 16,floatingPoint_ISR
ISR 17,alignmentCheck_ISR
ISR 18,machineCheck_ISR
ISR 19,SIMDFloatingPoint_ISR
ISR 20,virtualization_ISR
ISR 21,controlProtection_ISR

.global doubleFault_ISR
doubleFault_ISR:
  # Prepare for the ISR
  pusha

  # Move the exception error code back on top of the stack
  # so we can read it from the handler
  push 32(%esp)

  # Do the call
  call handleDoubleFault

  # Return
  popa

  # Remove the error code from the stack
  add $4, %esp

  iret

.global gpFault_ISR
gpFault_ISR:
  # Prepare for the ISR
  pusha

  # Move the exception error code back on top of the stack
  # so we can read it from the handler
  push 32(%esp)

  # Do the call
  call handleGeneralProtectionFault

  # Return
  popa

  # Remove the error code from the stack
  add $4, %esp

  iret
