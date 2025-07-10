{ pkgs }:

let
    common = import ./common.nix { inherit pkgs; };
in
    pkgs.stdenv.mkDerivation (common // {
        buildInputs = common.buildInputs;
        buildPhase = ''
            make cnc_translate
        '';
        installPhase = common.binInstallPhase; 
    })
