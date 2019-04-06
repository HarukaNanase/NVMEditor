#pragma once

#include "Util.h"
#include <fstream>
namespace NVMEditor {
	class NVMReader
	{
	public:
		NVMReader();
		~NVMReader();
		//Constructor with folder path, loads all navmeshes inside of it.
		NVMReader(std::string path);

		std::map <std::string, NavMesh> navmeshes;
		
		BYTE ReadNextByte(std::ifstream& file);
		char ReadNextChar(std::ifstream& file);
		ushort ReadNextUShort(std::ifstream& file);
		uint ReadNextUInt(std::ifstream& file);
		int ReadNextInt(std::ifstream& file);
		void reset_seeker();
		void set_seeker(uint bytes_from_start);


	};

}