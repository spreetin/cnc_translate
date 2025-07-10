{ pkgs }:

let
    common = import ./common.nix { inherit pkgs; };
in
    pkgs.stdenv.mkDerivation (common // {
        nativeBuildInputs = common.nativeBuildInputs
                        ++ common.guiNativeBuildInputs;
        buildInputs = common.buildInputs 
                    ++ common.guiBuildInputs;
        buildPhase = ''
            make cnc_gui
        '';
        installPhase = common.guiInstallPhase;
    })
