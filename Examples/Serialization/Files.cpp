#include "Files.h"

namespace Files {

	bool Exists(const char* filePath)
	{
		// ASSERT(filePath, "Invalid filePath!");
		return std::filesystem::status(filePath).type() == std::filesystem::file_type::regular;
	}

	Buffer LoadFile(const char* filePath) // #TODO Can I optimize return? Maybe using a reference instead?
	{
		// #TODO Validate adding null terminating character
		constexpr uint8_t addNullTerminatingChar = 1;

		Buffer buffer;

		if (!filePath)
			return buffer;

		FILE* filehandle;
		const errno_t error = fopen_s(&filehandle, filePath, "rb");

		if (filehandle)
		{
			fseek(filehandle, 0, SEEK_END);
			const uint32_t fileSizeBytes = ftell(filehandle);
			rewind(filehandle);

			buffer.Allocate(fileSizeBytes + addNullTerminatingChar);
			fread(buffer.Data(), fileSizeBytes, 1, filehandle);
			fclose(filehandle);

			if (addNullTerminatingChar)
			{
				buffer.Data()[buffer.SizeInBytes() - addNullTerminatingChar] = '\0';
			}
		}

		// #TODO Buffer memory is released in destructor
		// #TODO Assignment of return value copies to a new value using operator=() constructor
		return buffer;
	}

	Path FileName(const char* const filePath) // #TODO Consider a version with a bool to include file ".extension"
	{
		return Path(filePath).filename();
	}

	Path FileExtension(const char* filePath)
	{
		return Path(filePath).extension();
	}

	void CreateEmptyFile(const char* const filePath)
	{
		if (!Exists(filePath))
		{
			FILE* filehandle;
			errno_t error = fopen_s(&filehandle, filePath, "w");
			if (!error && filehandle)
			{
				fclose(filehandle);
			}
			else
			{
				// LOG_ERROR("{0} Could not create empty file {1}", __FUNCTION__, filePath);
			}
		}
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

}
