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
        "Core/Source/**.h",
        "Core/Vendor/imgui/imgui.cpp",
        "Core/Vendor/imgui/imgui_demo.cpp",
        "Core/Vendor/imgui/imgui_tables.cpp",
        "Core/Vendor/imgui/imgui_widgets.cpp",
        "Core/Vendor/imgui/imgui_draw.cpp",
        "Core/Vendor/imgui/backends/imgui_impl_opengl3.cpp",

        "Core/Vendor/imgui-rl/rlImGui.cpp"
    }

    filter "configurations:Debug or Release or Dist"
        includedirs { "Core/Vendor/Raylib/raylib-6.0_macOS/include", "Core/Vendor/imgui", "Core/Vendor/imgui-rl" }
        libdirs     { "Core/Vendor/Raylib/raylib-6.0_macOS/lib" }
        links       { "raylib" }

    filter "configurations:Web"
        includedirs { "Core/Vendor/Raylib/raylib-6.0_webassembly/include", "Core/Vendor/imgui", "Core/Vendor/imgui-rl" }
        toolset "clang"
        gccprefix "em"

        symbols "Off"
        optimize "Full"

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

    includedirs { "Core/Source", "Core/Vendor/imgui", "Core/Vendor/imgui-rl" }
    links { "Core" }

    filter "configurations:Debug or Release or Dist"
        includedirs { "Core/Vendor/Raylib/raylib-6.0_macOS/include", "Core/Vendor/imgui", "Core/Vendor/imgui-rl" }
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

        symbols "Off"
        optimize "Full"

        includedirs { "Core/Vendor/Raylib/raylib-6.0_webassembly/include", "Core/Vendor/imgui", "Core/Vendor/imgui-rl" }
        libdirs     { "Core/Vendor/Raylib/raylib-6.0_webassembly/lib" }
        links       { "raylib.web" }
        linkoptions
        {
            "-sUSE_GLFW=3",
            "-sASYNCIFY",
            "-sALLOW_MEMORY_GROWTH=1"
        }
        defines { "PLATFORM_WEB" }