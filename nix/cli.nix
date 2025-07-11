{ pkgs }:

let
    common = import ./common.nix { inherit pkgs; };
in
    pkgs.stdenv.mkDerivation (common // {
        cmakeFlags = common.cmakeFlags ++ [
            "-DBUILD_SHARED_LIB=OFF"
        ];
        buildInputs = common.buildInputs;
        buildPhase = ''
            make cnc_translate
        '';
    })
