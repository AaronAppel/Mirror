project "App"
	kind "ConsoleApp"
	location ""
	
	files
	{
		"**.h",
		"**.cpp",
		"**.txt",
	}

	includedirs
	{
		"%{wks.location}/Mirror/Source",
	}

	links
	{
		"cJSON",
		"Mirror",
	}
