project "Testing"
	kind "ConsoleApp"
	location ""
	rtti "On" -- Enabled for Mirror derived type checks at runtime
		
	files
	{
		"**.h",
		"**.cpp",
		"**.txt",
		"**.json",
	}

	includedirs
	{
		"%{wks.location}/Mirror/Source",
	}

	links
	{
		"Mirror",
	}

	defines
	{
		"MIRROR_TESTING=1"
	}