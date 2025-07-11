{ pkgs }:

let
    common = import ./common.nix { inherit pkgs; };
in
    pkgs.stdenv.mkDerivation (common // {
        cmakeFlags = common.cmakeFlags ++ [
            "-DBUILD_GUI_APP=ON"
            "-DBUILD_SHARED_LIB=OFF"
            "-DBUILD_STATIC_LIB=OFF"
        ];
        buildInputs = common.buildInputs 
                    ++ common.guiBuildInputs;
        buildPhase = ''
            make cnc_gui
        '';
    })
