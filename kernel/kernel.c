int main() {
  unsigned short *m_vga = (void *)0x000B8800;
  m_vga[0] = 0x0700 | 'H';
  m_vga[1] = 0x0700 | 'e';
  m_vga[2] = 0x0700 | 'l';
  m_vga[3] = 0x0700 | 'l';
  m_vga[4] = 0x0700 | 'o';
  m_vga[5] = 0x0700 | ' ';
  m_vga[6] = 0x0700 | 'w';
  m_vga[7] = 0x0700 | 'o';
  m_vga[8] = 0x0700 | 'r';
  m_vga[9] = 0x0700 | 'l';
  m_vga[10] = 0x0700 | 'd';
  m_vga[11] = 0x0700 | '!';
}
