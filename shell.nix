{ pkgs ? import <nixpkgs> { } }:
let
    common = import ./nix/comon.nix { inherit pkgs; };
in
pkgs.mkShell (common // {
    nativeBuildInputs = common.nativeBuildInputs
                        + common.guiNativeBuildInputs;
    buildInputs = common.buildInputs
                + common.guiBuildInputs;
    shellHook = ''
        bashDir=$(mktemp -d)
        makeWrapper "$(type -p bash)" "$bashDir/bash" "''${qtWrapperArgs[@]}"
        exec "$bashDir/bash"
    '';
})
