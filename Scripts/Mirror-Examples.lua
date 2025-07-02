workspace "Examples"

	-- Workspace specific settings
	startproject "Examples"
	location "../"

	-- Solution wide shared settings
	configurations { "Debug", "Release", }
	flags { "MultiProcessorCompile" }
	rtti "Off"
	language "C++"
	cppdialect "C++17"

	OutputDir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"
		
	filter "configurations:*"
		buildoptions { "/permissive" } -- No conformance
		-- buildoptions { "/permissive-" } -- Strict conformance
		
	filter "configurations:Debug"
		architecture "x64"
		targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
		objdir ("%{wks.location}/bin/int/" .. OutputDir .. "/%{prj.name}")
		runtime "Debug"
		symbols "on"
		optimize "off"
		
	filter "configurations:Release"
		architecture "x64"
		targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
		objdir ("%{wks.location}/bin/int/" .. OutputDir .. "/%{prj.name}")
		runtime "Release"
		symbols "off"
		optimize "on"
	
project "Mirror"
	kind "StaticLib"
	location "%{wks.location}/Examples"
	
	files
	{
		"%{wks.location}/Source/**.h",
		"%{wks.location}/Source/**.cpp"
	}
	
project "Serialization"
	kind "StaticLib"
	location "%{wks.location}/Examples/Serialization"
	
	files
	{
		"%{wks.location}/Examples/Serialization/**.h",
		"%{wks.location}/Examples/Serialization/**.cpp"
	}

project "Examples"
	kind "ConsoleApp"
	location "%{wks.location}/Examples"
	rtti "On" -- Enabled for Mirror derived type checks at runtime
	
	files
	{
		"%{wks.location}/Examples/Source/**.h",
		"%{wks.location}/Examples/Source/**.cpp",
		
		"%{wks.location}/Examples/cJSON/**.h",
		"%{wks.location}/Examples/cJSON/**.cpp"
	}
	
	includedirs
	{
		"%{wks.location}/Source",
		"%{wks.location}/Examples/cJSON",
	}

	links
	{
		"Mirror"
	}
	