local raylibDir = "Core/Vendor/Raylib/raylib-6.0_macOS"
if os.target() == "windows" then
    raylibDir = "Core/Vendor/Raylib/raylib-6.0_win64_msvc16"
end

workspace "OLCCodeJam26"
    configurations { "Debug", "Release", "Dist" }
    startproject "App"

    filter "system:macosx"
        architecture "arm64"
    filter "system:not macosx"
        architecture "x64"
    filter "system:windows"
        systemversion "latest"
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        optimize "Off"
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
    filter "configurations:Dist"
        defines { "NDEBUG", "DIST" }
        optimize "Full"
        symbols "Off"
    filter {}

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

    includedirs
    {
        "Core/Source",
        raylibDir .. "/include",
        "Core/Vendor/imgui",
        "Core/Vendor/imgui-rl"
    }

project "App"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++23"

    targetdir "bin/%{cfg.buildcfg}"
    debugdir  "%{cfg.targetdir}"

    files
    {
        "App/Source/**.cpp",
        "App/Source/**.h"
    }

    includedirs
    {
        "Core/Source",
        raylibDir .. "/include",
        "Core/Vendor/imgui",
        "Core/Vendor/imgui-rl",
        "App/Assets"
    }

    libdirs { raylibDir .. "/lib" }
    links { "Core", "raylib" }

    filter "system:macosx"
        links
        {
            "OpenGL.framework", "Cocoa.framework", "IOKit.framework",
            "CoreVideo.framework", "QuartzCore.framework"
        }
    filter "system:windows"
        links { "opengl32", "gdi32", "winmm", "user32", "shell32" }
    filter {}

    postbuildcommands { "{COPYDIR} %{wks.location}/App/Assets %{cfg.targetdir}" }