{ pkgs }:

let
    common = import ./common.nix { inherit pkgs; };
in
    pkgs.stdenv.mkDerivation (common // {
        cmakeFlags = common.cmakeFlags ++ [
            "-DBUILD_GUI_APP=ON"
        ];
        buildInputs = common.buildInputs 
                    ++ common.guiBuildInputs;
    })
