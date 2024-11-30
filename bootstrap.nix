let
  overlays = [
    (import ./nix/wlroots-overlay.nix)
  ];
  pkgs = import <nixpkgs> { overlays = overlays; };
in
pkgs.mkShell {
  buildInputs = [
    pkgs.pkg-config
    pkgs.wlroots
    pkgs.wayland.dev
    pkgs.wayland-scanner
    pkgs.libxkbcommon.dev
    pkgs.pixman
    pkgs.wayland-protocols
    pkgs.ninja
    pkgs.udev.dev
  ];
}
