#include "Utilities.h"

#include <cstring>
#include <stdio.h> // For fwrite, fclose, and general file I/O

char* LoadFile(const char* filePath)
{
	if (!filePath)
		return nullptr;

	FILE* filehandle;
	const errno_t error = fopen_s(&filehandle, filePath, "rb");

	char* buffer = nullptr;
	if (filehandle)
	{
		fseek(filehandle, 0, SEEK_END);
		const long fileSizeBytes = ftell(filehandle);
		rewind(filehandle);

		buffer = new char[fileSizeBytes];

		fread(buffer, fileSizeBytes, 1, filehandle);
		fclose(filehandle);
	}

	return buffer; // #TODO Buffer memory is released in destructor
}

void WriteStringToFile(const char* const fileData, const char* const filePath)
{
	if (!filePath)
	{
		// LOG_ERROR("{0} Null argument passed!", __FUNCTION__);
		return;
	}

	FILE* filehandle;
	errno_t error = fopen_s(&filehandle, filePath, "w+");
	if (!error && filehandle)
	{
		fwrite(fileData, 1, strlen(fileData), filehandle);
		fclose(filehandle);
	}
	else
	{
		// LOG_ERROR("{0} Could not open file for write {1}", __FUNCTION__, filePath);
	}
}