.global pageFault_ISR
pageFault_ISR:
  # Prepare for the ISR
  pusha

  # Move the pagefaulted address back on top of the stack
  # so we can read it from the handler
  push 32(%esp)

  # Write the error code to the stack for the handler
  mov %cr2, %eax
  push %eax

  # Do the pagefault
  call handlePageFault

  # Invalidate any entries in the TLB
  invlpg 8(%esp)

  # Pop the arguments to put the stack back the way it was
  add $8, %esp

  # Grab all of the stashed registers
  popa

  # Remove the faulted address that was pushed before the handler was called
  add $4, %esp

  iret
