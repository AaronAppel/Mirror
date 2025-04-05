workspace "Mirror"

	-- Workspace specific settings
	startproject "Examples"
	location "../"

	-- Solution wide shared settings
	configurations { "Debug", "Release", }
	flags { "MultiProcessorCompile" }
	rtti "Off"
	staticruntime "off" -- https://premake.github.io/docs/staticruntime
	language "C++"
	cppdialect "C++17"

	OutputDir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"
		
	filter "configurations:*"
		-- buildoptions { "/permissive-" } -- Strict conformance
		buildoptions { "/permissive" } -- No conformance
	
	filter "configurations:*"
		buildoptions { "" }
		
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
	
	include "../Projects/cJSON/Build-cJSON.lua"
	include "../Projects/Examples/Build-Examples.lua"
	include "../Projects/Testing/Build-Testing.lua"
	include "../Mirror/Build-Mirror.lua"
	