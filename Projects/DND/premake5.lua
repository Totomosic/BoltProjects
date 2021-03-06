project "DND"
    location ""
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir (SolutionDir .. "bin/" .. outputdir .. "/DND")
    objdir (SolutionDir .. "bin-int/" .. outputdir .. "/DND")

    pchheader "dndpch.h"
    pchsource "src/dndpch.cpp"
    
    files
    {
        "src/**.h",
        "src/**.cpp",
        "src/**.hpp",
        "src/**.c"
    }
    
    includedirs
    {
        "../../%{BoltDir}%{IncludeDirs.Bolt}",
        "../../%{BoltDir}%{IncludeDirs.GLFW}",
        "../../%{BoltDir}%{IncludeDirs.Glad}",
        "../../%{BoltDir}%{IncludeDirs.ImGui}",
        "../../%{BoltDir}%{IncludeDirs.spdlog}",
        "../../%{BoltDir}%{IncludeDirs.FreeType}",
        "../../%{BoltDir}%{IncludeDirs.FreeTypeGL}",
        "../../%{BoltDir}%{IncludeDirs.Lua}",
        "src"
    }

    links
    {
        "Bolt-Core",
        "opengl32.lib",
        "ws2_32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "BLT_PLATFORM_WINDOWS",
            "BLT_BUILD_STATIC"
        }

    filter "configurations:Debug"
        defines "BLT_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "BLT_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "BLT_DIST"
        runtime "Release"
        optimize "on"