{
  pkgs ? import <nixpkgs> {},
  i686 ? import <nixpkgs> { crossSystem.config = "i686-elf"; }
}:

i686.mkShell {
  nativeBuildInputs = [
    pkgs.gnumake
    pkgs.qemu_kvm
    pkgs.mtools
    pkgs.parted
  ];
}
