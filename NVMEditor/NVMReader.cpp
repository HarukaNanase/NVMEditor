#include "pch.h"
#include "NVMReader.h"


using namespace NVMEditor;
namespace fs = std::filesystem;
using namespace std;
NVMReader::NVMReader()
{
}


NVMReader::~NVMReader()
{
}

NVMEditor::NVMReader::NVMReader(string path)
{
	cout << "Found files:" << endl;
	for (const auto& dir_entry : fs::directory_iterator(path)) {
		if (dir_entry.is_directory() && dir_entry.path().filename() == NVM_DIRECTORY) {

			for (const auto& nvm_entry : fs::directory_iterator(dir_entry.path().string())) {

				cout << nvm_entry.path().filename() << " Extension: " <<
					nvm_entry.path().extension() <<  endl;

				if (nvm_entry.path().extension().string() == NVM_EXTENSION) {
				    // open files into memory at the start or save into map with null and spawn it when needed?	
					//unique_ptr<ifstream> nvm_file = make_unique<ifstream>(ifstream(nvm_entry.path().string()));
					ifstream* nvm_file = new ifstream(nvm_entry.path().string());
					this->navmesh_files.emplace(nvm_entry.path().string(), nvm_file);
					// if we want to create streams right now, insert the unique_ptr above into the map, else insert nullptr.
					//pair<std::string, unique_ptr<ifstream>> entry(nvm_entry.path().string(), nvm_file);
					//this->navmesh_files.insert(make_pair(nvm_entry.path().string(), nvm_file));

				}
			}
			break;
		}
	}
}

void NVMEditor::NVMReader::create_navmeshes()
{
	//create navmesh structures from file.
	for (auto& kv : this->navmesh_files) {
		auto nvm_file = kv.second;
		cout << nvm_file->is_open() << endl;
		//make sure file is open and set to 0
		//reset_seeker(nvm_file);
		NavMesh nvm;
		//read the first twelve bytes which compose the header JMXNVM 1000
		for (int i = 0; i < 12; i++)
			nvm.metadata.header[i] = read_next_char(nvm_file);
		//read 2 bytes that have the count of nav entries
		nvm.metadata.nav_entries = read_next_ushort(nvm_file);
		
		//for each navigation entry, read the corresponding data.
		for (ushort i = 0; i < nvm.metadata.nav_entries; i++) {
			NavigationEntry nav_entry;
			//read 4 bytes for the id
			nav_entry.id = read_next_uint(nvm_file);
			//read 12 bytes for the Vector3 (x,y,z)
			nav_entry.position = read_next_vec3(nvm_file);
			//next 2 bytes indicate the collision flag. 0x0000 = no collision ; 0xFFFF = collision
			nav_entry.collision = read_next_ushort(nvm_file);
			//yaw of...? 8 bytes
			nav_entry.yaw = read_next_float(nvm_file);
			//unique_id is 2 bytes
			nav_entry.unique_id = read_next_ushort(nvm_file);
			//unknown short that i have no idea what it is being used for - 2 bytes
			nav_entry.unknown_0 = read_next_ushort(nvm_file);
			//event zone flag, 0x0000 = no collision while using CTF suit, 0xFFFF = collision using ctf suit
			nav_entry.event_zone = read_next_ushort(nvm_file);
			//region id, id of the region this navigation entry belongs to , will complete with info from db
			nav_entry.region_id = read_next_ushort(nvm_file);
			//number of mountpoints in this navigation entry
			nav_entry.mount_point_entries = read_next_ushort(nvm_file);
			for (ushort u = 0; u < nav_entry.mount_point_entries; u++) {
				MountPoint m;
				for (uint n = 0; n < sizeof(m.mount_point); n++) {
					m.mount_point[n] = read_next_byte(nvm_file);
				}
				nav_entry.mount_points.push_back(m);
			}

			nvm.nav_entries.emplace(nav_entry.id, nav_entry);

		}

		this->navmeshes.emplace(kv.first, nvm);
	}

}

BYTE NVMEditor::NVMReader::read_next_byte(ifstream* file)
{
	char _b;
	file->read(&_b, 1);
	return (BYTE)_b;
}

char NVMEditor::NVMReader::read_next_char(ifstream* file)
{
	char _c;
	file->read(&_c, 1);
	return _c;
}

ushort NVMEditor::NVMReader::read_next_ushort(ifstream* file)
{
	char _u[2];
	file->read(_u, 2);
	ushort __u = ((_u[1] << 8) & 0xFF00) | (_u[0] & 0xFF);
	return __u;
}

uint NVMEditor::NVMReader::read_next_uint(ifstream* file)
{
	uint32_t _ui;
	file->read(reinterpret_cast<char *>(&_ui), sizeof(_ui));

	return _ui;
}

int NVMEditor::NVMReader::read_next_int(ifstream* file)
{
	int32_t _i;
	file->read(reinterpret_cast<char *>(&_i), sizeof(_i));
	return _i;
}

float NVMEditor::NVMReader::read_next_float(ifstream* file)
{
	float _f;
	file->read(reinterpret_cast<char*>(&_f), sizeof(_f));
	return _f;
}

Vector2 NVMEditor::NVMReader::read_next_vec2(ifstream* file)
{
	Vector2 _v2;
	file->read(reinterpret_cast<char*>(&_v2), sizeof(_v2));
	return _v2;
}

Vector3 NVMEditor::NVMReader::read_next_vec3(ifstream* file)
{
	
	int x = read_next_int(file);
	int y = read_next_int(file);
	int z = read_next_int(file);

	//file->read(reinterpret_cast<char*>(&_v3), sizeof(_v3));

	return Vector3{ x,y,z };
}

void NVMEditor::NVMReader::reset_seeker(ifstream* file)
{
	file->seekg(0, file->beg);
}

void NVMEditor::NVMReader::set_seeker(ifstream* file, uint bytes_from_start)
{
	file->seekg(bytes_from_start, file->beg);
}


long NVMEditor::NVMReader::bytes_left_in_file(ifstream* file)
{
	//save current seeker position
	long cur_pos = file->tellg();
	file->seekg(0, file->end);
	long file_size = file->tellg();
	file->seekg(cur_pos, file->beg);

	return file_size - cur_pos;
}
