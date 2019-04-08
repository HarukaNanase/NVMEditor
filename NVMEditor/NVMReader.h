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
		//std::map <std::string, std::unique_ptr<std::ifstream>> navmesh_files;
		std::map <std::string, std::ifstream*> navmesh_files;
		void create_navmeshes();


		BYTE read_next_byte(std::ifstream* file);
		char read_next_char(std::ifstream* file);
		ushort read_next_ushort(std::ifstream* file);
		uint read_next_uint(std::ifstream* file);
		int read_next_int(std::ifstream* file);
		float read_next_float(std::ifstream* file);
		Vector2 read_next_vec2(std::ifstream* file);
		Vector3 read_next_vec3(std::ifstream* file);
		void reset_seeker(std::ifstream* file);
		void set_seeker(std::ifstream* file, uint bytes_from_start);
		long bytes_left_in_file(std::ifstream* file);


	};

}