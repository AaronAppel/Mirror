
#include <fstream>
#include <iostream>
#include <string>

#include "MIR_Mirror.h" // Mir::TypeInfo, Mir::Info, Mir::Id

#include "Main.h"
#include "ForMirror.h" // For Mir::Id<>()

void FileIO()
{
	// Simple example of writing and reading type data to a file
	std::ofstream myfile;

	{	// Serialize
		myfile.open("example.txt");
		myfile << Mir::GetId<int>();
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
				case Mir::GetId<char>():
					{
						const char a = line[1];
						std::cout << a << '\n';
					}
					break;

				case Mir::GetId<std::string>():
					{
						const std::string a = line.substr(1, line.size() - 1);
						std::cout << a << '\n';
					}
					break;

				case Mir::GetId<int>():
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