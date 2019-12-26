workspace "BoltProjects"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

-- Path to directory containing Bolt-Core
BoltDir = "Bolt/"
include (BoltDir .. "BoltInclude.lua")

ProjectsDir = "Projects/"

group("Projects")
include (ProjectsDir .. "Aimbooster")
include (ProjectsDir .. "Blockstream")
include (ProjectsDir .. "DND")
include (ProjectsDir .. "Meteors")
include (ProjectsDir .. "Minecraft")
include (ProjectsDir .. "PBRExample")
include (ProjectsDir .. "ECSTest")