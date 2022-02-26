workspace "Vox"
	architecture "x86"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Vox"
	location "vox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	libdirs
	{
		"Dependencies/GLFW/lib-vc2019",
		"Dependencies/GLEW/lib/Release/Win32"
	}
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.inl"
	}

	includedirs
	{
		"Dependencies/GLFW/include",
		"Dependencies/GLEW/include",
		"%{prj.name}/src",
		"%{prj.name}/src/stb_image"
	}

	links
	{
		"glfw3.lib",
		"opengl32.lib",
		"User32.lib",
		"Gdi32.lib",
		"Shell32.lib",
		"glew32s.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "off"
		systemversion "latest"

		defines
		{
			"WIN32"
		}
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
	
	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		