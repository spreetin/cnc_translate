{ pkgs }:

let
    common = import ./common.nix { inherit pkgs; };
in
    pkgs.stdenv.mkDerivation (common // {
        buildPhase = ''
            make cnc_parse_shared
            make cnc_parse_static
        '';
        installPhase = + common.sharedLibInstallPhase
                    + common.staticLibInstallPhase
                    + common.includeInstallPhase;
    })
