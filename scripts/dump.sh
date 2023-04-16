outfile=/tmp/memory.img

nc localhost 4444 -w1 <<EOF
{ "execute": "qmp_capabilities" }
{"execute":"dump-guest-memory","arguments":{"paging":false,"protocol":"file:$outfile"}}
EOF

chmod 644 $outfile
# Must be run with system objcopy, not the cross chain objcopy
objcopy --dump-section load1=ram.bin $outfile -I elf32-little
od -t x4 ram.bin -A x | less
