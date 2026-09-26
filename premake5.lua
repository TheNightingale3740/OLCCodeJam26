workspace "OLCCodeJam26"
    configurations { "Debug", "Release", "Dist", "Web" }
    
    filter "configurations:Debug or Release or Dist"
        architecture "arm64"

project "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++23"

    files
    {
        "Core/Source/**.cpp",
        "Core/Source/**.h"
    }

    filter "configurations:Debug or Release or Dist"
        includedirs { "Core/Vendor/Raylib/raylib-6.0_macOS/include" }
        libdirs     { "Core/Vendor/Raylib/raylib-6.0_macOS/lib" }
        links       { "raylib" }

    filter "configurations:Web"
        includedirs { "Core/Vendor/Raylib/raylib-6.0_webassembly/include" }
        toolset "clang"
        gccprefix "em"
        defines { "PLATFORM_WEB" }

project "App"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++23"

    files
    {
        "App/Source/**.cpp",
        "App/Source/**.h"
    }

    includedirs { "Core/Source" }
    links { "Core" }

    filter "configurations:Debug or Release or Dist"
        includedirs { "Core/Vendor/Raylib/raylib-6.0_macOS/include" }
        libdirs     { "Core/Vendor/Raylib/raylib-6.0_macOS/lib" }
        links       { "raylib" }

    filter { "system:macosx", "configurations:not Web" }
        links
        {
            "OpenGL.framework", "Cocoa.framework", "IOKit.framework",
            "CoreVideo.framework", "QuartzCore.framework"
        }

    filter "configurations:Web"
        toolset "clang"
        gccprefix "em"
        targetextension ".html"
        includedirs { "Core/Vendor/Raylib/raylib-6.0_webassembly/include" }
        libdirs     { "Core/Vendor/Raylib/raylib-6.0_webassembly/lib" }
        links       { "raylib.web" }
        linkoptions
        {
            "-sUSE_GLFW=3",
            "-sASYNCIFY",
            "-sALLOW_MEMORY_GROWTH=1"
        }
        defines { "PLATFORM_WEB" }