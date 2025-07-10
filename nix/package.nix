{ pkgs }:

let
    common = import ./common.nix { inherit pkgs; };
in
    pkgs.stdenv.mkDerivation (common // {
        buildInputs = common.buildInputs 
                    ++ common.guiBuildInputs;
        buildPhase = ''
            make
        '';
        installPhase = common.binInstallPhase 
                    + common.guiInstallPhase
                    + common.sharedLibInstallPhase
                    + common.staticLibInstallPhase
                    + common.includeInstallPhase;
    })
