self: super: {
  wlroots_0_16_2 = super.stdenv.mkDerivation {
    pname = "wlroots";
    version = "0.16.2";
    src = self.fetchFromGitLab {
      domain = "gitlab.freedesktop.org";
      owner = "wlroots";
      repo = "wlroots";
      rev = "0.16.2";
      hash = "sha256-JeDDYinio14BOl6CbzAPnJDOnrk4vgGNMN++rcy2ItQ=";
    };
    nativeBuildInputs = [
      self.meson
      self.ninja
      self.pkg-config
      self.wayland-scanner
      self.glslang
      self.hwdata
    ];
    buildInputs = [
      self.libGL
      self.libcap
      self.libinput
      self.libpng
      self.libxkbcommon
      self.mesa
      self.pixman
      self.seatd
      self.vulkan-loader
      self.wayland
      self.wayland-protocols
      self.xorg.libX11
      self.xorg.xcbutilerrors
      self.xorg.xcbutilimage
      self.xorg.xcbutilrenderutil
      self.xorg.xcbutilwm
      self.ffmpeg_4
      self.libliftoff
    ];
    depsBuildBuild = [ self.pkg-config ];
    mesonFlags = [
      "-Dxwayland=disabled"
    ];
    meta = {
      description = "Modular Wayland compositor library";
      license = self.lib.licenses.mit;
      platforms = self.lib.platforms.linux;
    };
  };

  # Override the default wlroots to use the new version
  wlroots = self.wlroots_0_16_2;
}
