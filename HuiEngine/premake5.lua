project "HuiEngine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "Default"
    buildoptions { "/utf-8" }

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/glm/glm/**.hpp",
        "vendor/glm/glm/**.inl",
    }

    includedirs
    {
        "src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.tinyobjloader}",
        "%{IncludeDir.VulkanSDK}"
    }

    links
    {
        "ImGui",
    }
    
    filter "system:windows"
        systemversion "latest"

        defines
        {
            "GLFW_INCLUDE_NONE"
        }

        links
        {
            "%{Library.Vulkan}"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

        links
        {
            "%{Library.GLFW_Debug}",
            "%{Library.ShaderC_Debug}",
            "%{Library.SPIRV_Cross_Debug}",
            "%{Library.SPIRV_Cross_GLSL_Debug}"
        }

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

        links
        {
            "%{Library.GLFW_Release}",
            "%{Library.ShaderC_Release}",
            "%{Library.SPIRV_Cross_Release}",
            "%{Library.SPIRV_Cross_GLSL_Release}"
        }

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"

        links
        {
            "%{Library.GLFW_Release}",
            "%{Library.ShaderC_Release}",
            "%{Library.SPIRV_Cross_Release}",
            "%{Library.SPIRV_Cross_GLSL_Release}"
        }
