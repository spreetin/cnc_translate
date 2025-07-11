{
  description = "CNC code parser and translator";

  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
        imports = [
            # To import a flake module
            # 1. Add foo to inputs
            # 2. Add foo as a parameter to the outputs function
            # 3. Add here: foo.flakeModule
        ];
        systems = [ "x86_64-linux" "aarch64-linux" "aarch64-darwin" "x86_64-darwin" ];
        perSystem = { config, self', inputs', pkgs, system, ... }: 
        {

            devShells.default = import ./shell.nix { inherit pkgs; };

            packages = {
                default = import ./nix/package.nix { inherit pkgs; };
                shared = import ./nix/shared.nix { inherit pkgs; };
                static = import ./nix/static.nix { inherit pkgs; };
                gui = import ./nix/gui.nix { inherit pkgs; };
                cli = import ./nix/cli.nix { inherit pkgs; };
                library = import ./nix/library.nix { inherit pkgs; };
            };
        };
        flake = {
            # The usual flake attributes can be defined here, including system-
            # agnostic ones like nixosModule and system-enumerating ones, although
            # those are more easily expressed in perSystem.
        };
    };
}
