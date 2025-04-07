
#include <fstream>
#include <iostream>
#include <string>

#include "MIR_Mirror.h" // Mirror::TypeInfo, Mirror::Info, Mirror::Id

#include "Main.h"
#include "ForMirror.h" // For Mirror::Id<>()

void FileIO()
{
	// Simple example of writing and reading type data to a file
	std::ofstream myfile;

	{	// Serialize
		myfile.open("example.txt");
		myfile << Mirror::Id<int>();
		myfile << "1234567890";
		myfile.close();
	}

	// Example file data for type int
	// "û1234567890"

	{	// Deserialize
		std::ifstream myfile("example.txt");

		if (myfile.is_open())
		{
			std::string line;
			while (std::getline(myfile, line))
			{
				uint8_t id = static_cast<uint8_t>(line[0]);
				switch (id)
				{
				case Mirror::Id<char>():
					{
						const char a = line[1];
						std::cout << a << '\n';
					}
					break;

				case Mirror::Id<std::string>():
					{
						const std::string a = line.substr(1, line.size() - 1);
						std::cout << a << '\n';
					}
					break;

				case Mirror::Id<int>():
					{
						const int a = std::stoi(line.substr(1, line.size() - 1));
						std::cout << a << '\n';
					}
					break;
				}
			}
			myfile.close();
		}
	}

	std::cout << "\n";
}