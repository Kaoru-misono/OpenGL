workspace "LearnOpenGL"
	architecture "x64"
	startproject "LearnOpenGL"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include directories
IncludeDir = {}
IncludeDir["GLFW"] = "LearnOpenGL/vendor/GLFW/include"
IncludeDir["Glad"] = "LearnOpenGL/vendor/Glad/include"
IncludeDir["glm"] = "LearnOpenGL/vendor/glm"
IncludeDir["stb_image"] = "LearnOpenGL/vendor/stb_image"
IncludeDir["assimp"] = "LearnOpenGL/vendor/assimp/include"



include "LearnOpenGL/vendor/GLFW"
include "LearnOpenGL/vendor/Glad"
include "LearnOpenGL/vendor/assimp"


project "LearnOpenGL"
	location "LearnOpenGL"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.assimp}"

	}

	links
	{
		"GLFW",
		"Glad",
		"assimp",
		"opengl32.lib"
		
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"