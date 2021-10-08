# https://wiki.osdev.org/Global_Descriptor_Table
# The GDT lists all of the memory segments available in protected mode. It is
# loaded into the processor by executing lgdt (gdt_descriptor), where
# gdt_descriptor is the address of the gdt descriptor in memory.

# The first entry in the GDT is the null descriptor, it can't be used, but has
# to be allocated. We can construct the GDT descriptor in that space to save
# some bytes
gdt:
	# The GDT descriptor contains a 2 byte size followed by a 4 byte offset to the
	# first first entry in the table. The size is expressed as the last address
	# that belongs to the table (e.g. a table of 16 bytes would have size 0xFF).
	# GDT Entry 0x00
	.word gdt_end - gdt - 1 # Size
	.long gdt								# Offset
	.word 0x0000            # Padding

	# Define our first segment for code from address 0x00000000 to page 0xFFFFF
	# Access:
	# GDT Entry 0x08
	.word  0xFFFF    # Limit (4GB)
	.word  0x0000    # Base 15-0
	.byte 0x00       # Base 23-16
	.byte 0b10011010 # Access
	.byte 0b11001111 # Flags (7-4) / Limit (3-0)
	.byte 0x00       # Base 31-24

	# Second segment has the same range, but different permissions. This works because
	# segments are allowed to overlap
	# GDT Entry 0x10
	.word  0xFFFF    # Limit (4GB)
	.word  0x0000    # Base 15-0
	.byte 0x00       # Base 23-16
	.byte 0b10010010 # Access
	.byte 0b11001111 # Flags Limit
	.byte 0x00       # Base 31-24
gdt_end:
