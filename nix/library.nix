{ pkgs }:

let
    common = import ./common.nix { inherit pkgs; };
in
    pkgs.stdenv.mkDerivation (common // {
        cmakeFlags = common.cmakeFlags ++ [
            "-DBUILD_CLI_APP=OFF"
        ];
        buildPhase = ''
            make cnc_parse_shared
            make cnc_parse_static
        '';
    })
