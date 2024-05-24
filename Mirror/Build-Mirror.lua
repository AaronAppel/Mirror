project "Mirror"
	kind "StaticLib"
	location ""
	
	files
	{
		"**.h",
		"**.cpp",
	}

	defines
	{
		-- #TODO Enable asserts
		"MIRROR_TESTING"
	}