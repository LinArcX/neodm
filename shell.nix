let
  pkgs-2020-03-23 = import (fetchTarball https://github.com/NixOS/nixpkgs/archive/793c1b5c72abbfed2f98add0811a022fc713dbf3.tar.gz) {
     overlays = [ (import ./assets/packages/nix/overlay/aria.nix) ];
  };
in
  pkgs-2020-03-23.clangStdenv.mkDerivation rec {
    pname   = "neodm";
    version = "1.0.0";
    name    = "${pname}${version}";

    nativeBuildInputs = [
      pkgs-2020-03-23.pkg-config
      pkgs-2020-03-23.cmake
    ];

    buildInputs = [
      pkgs-2020-03-23.clang-tools
      pkgs-2020-03-23.clang-analyzer

      pkgs-2020-03-23.man
      pkgs-2020-03-23.lldb_9
      pkgs-2020-03-23.man-db
      pkgs-2020-03-23.man-pages
      pkgs-2020-03-23.clang-manpages
      pkgs-2020-03-23.posix_man_pages

      pkgs-2020-03-23.ncurses
      pkgs-2020-03-23.sqlite
      pkgs-2020-03-23.cdk
      pkgs-2020-03-23.aria

      #(pkgs-2020-03-23.callPackage ./nix/aria2.nix { })
      #pkgs-2020-03-23.newt
    ];

    FONTCONFIG_FILE = "${pkgs-2020-03-23.fontconfig.out}/etc/fonts/fonts.conf";
    LOCALE_ARCHIVE="${pkgs-2020-03-23.glibcLocales}/lib/locale/locale-archive";

    shellHook = ''
      export NAME=${pname}
      export CMAKE=${pkgs-2020-03-23.cmake}/bin/cmake
      export CLANGD=${pkgs-2020-03-23.clang-tools}/bin/clangd
      export LLDB_VSCODE=${pkgs-2020-03-23.lldb_9}/bin/lldb-vscode
    '';
  }
