{ pkgs }:

{
    pname = "CNC Translate";
    version = "0.0.1";
    src = pkgs.lib.cleanSource ../.;
    cmakeFlags = [
        "-DCMAKE_PREFIX_PATH=${pkgs.qt6.qtbase}"
        "-DCMAKE_BUILD_TYPE=Release"
    ];
    nativeBuildInputs = with pkgs; [
        pkg-config
        cmake
        qt6.qtbase
        qt6.full
        qt6.wrapQtAppsHook
        makeWrapper
    ];
    buildInputs = with pkgs; [ 
        ctre
    ];
    guiBuildInputs = with pkgs; [
        qt6.qtbase
        qt6.full
    ];
}
