workspace "OLCCodeJam26"
    configurations { "Debug", "Release", "Dist" }

    filter "system:macosx"
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

project "App"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++23"

    targetdir "bin/%{cfg.buildcfg}"

    files
    {
        "App/Source/**.cpp",
        "App/Source/**.h"
    }

    includedirs { "Core/Source", "Core/Vendor/imgui", "Core/Vendor/imgui-rl", "App/Assets" }
    links { "Core" }

    filter "configurations:Debug or Release or Dist"
        includedirs { "Core/Vendor/Raylib/raylib-6.0_macOS/include", "Core/Vendor/imgui", "Core/Vendor/imgui-rl" }
        libdirs     { "Core/Vendor/Raylib/raylib-6.0_macOS/lib" }
        links       { "raylib" }

    filter "system:macosx"
        links
        {
            "OpenGL.framework", "Cocoa.framework", "IOKit.framework",
            "CoreVideo.framework", "QuartzCore.framework"
        }

    postbuildcommands { "{COPYDIR} %{wks.location}/App/Assets %{cfg.targetdir}" }