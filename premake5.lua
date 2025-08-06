include "Dependencies.lua"

workspace "HuiRayTracing"
    architecture "x64"
    startproject "HuiEngine"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencis"
        include "HuiEngine/vendor/ImGui"
group ""

group "Core"
    include "HuiEngine"
group ""
