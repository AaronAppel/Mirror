project "Examples"
	kind "ConsoleApp"
	location ""
	rtti "On" -- Enabled for Mirror derived type checks at runtime
	
	files
	{
		"**.h",
		"**.cpp",
	}

	includedirs
	{
		"%{wks.location}/Projects/cJSON/Source",
		"%{wks.location}/Mirror/Source",
	}

	links
	{
		"cJSON",
		"Mirror",
	}
