{ pkgs }:

let
    common = import ./common.nix { inherit pkgs; };
in
    pkgs.stdenv.mkDerivation (common // {
        nativeBuildInputs = common.nativeBuildInputs ;
        buildInputs = common.buildInputs;
        buildPhase = ''
            make cnc_translate
        '';
        installPhase = common.binInstallPhase; 
    })
