{ pkgs }:

let
    common = import ./common.nix { inherit pkgs; };
in
    pkgs.stdenv.mkDerivation (common // {
        buildPhase = ''
            make cnc_parse_shared
        '';
        installPhase = common.sharedLibInstallPhase
                    + common.includeInstallPhase;
    })
