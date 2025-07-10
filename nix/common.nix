{ pkgs }:

{
    pname = "CNC Translate";
    version = "0.0.1";
    src = pkgs.lib.cleanSource ../.;
    nativeBuildInputs = with pkgs; [
        pkg-config
        cmake
    ];
    guiNativeBuildInputs = with pkgs; [
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
    configurePhase = ''
        mkdir build
        cd build
        cmake -B . -S ../ -DCMAKE_BUILD_TYPE=Release
    '';
    binInstallPhase = ''
        mkdir -p $out/bin
        cp cnc_translate $out/bin
    '';
    guiInstallPhase = ''
        mkdir -p $out/bin
        cp cnc_gui/cnc_gui $out/bin
    '';
    sharedLibInstallPhase = ''
        mkdir -p $out/lib
        cp libcnc_parse_shared.so $out/lib/libcnc_parse.so
    '';
    staticLibInstallPhase = ''
        mkdir -p $out/lib
        cp libcnc_parse_static.a $out/lib/libcnc_parse.a
    '';
    includeInstallPhase = ''
        mkdir -p $out/include/{brands,tokens,generator,parser}
        cp ../libcnc_parse.h $out/include
        cp ../brands/brands.h ../brands/parameters.h ../brands/parameter_definitions.h $out/include/brands
        cp ../generator/generator.h $out/include/generator
        cp ../parser/parser.h ../parser/parse_node.h $out/include/parser
        cp ../tokens/g_codes.h ../tokens/m_codes.h $out/include/tokens
    '';
}
