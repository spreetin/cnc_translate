{ pkgs }:
let
    common = import ./nix/common.nix { inherit pkgs; };
in
pkgs.mkShell {
    nativeBuildInputs = common.nativeBuildInputs;
    buildInputs = common.buildInputs
                ++ common.guiBuildInputs
                ++ [ pkgs.bashInteractive ];
    shellHook = ''
        bashDir=$(mktemp -d)
        makeWrapper "$(type -p bash)" "$bashDir/bash" "''${qtWrapperArgs[@]}"
        exec "$bashDir/bash"
    '';
}
