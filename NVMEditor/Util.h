#pragma once

#include <vector>
#include <map>
#include <Windows.h>
#include <memory>
namespace NVMEditor {


#define NVM_EXTENSION ".nvm"
#define NVM_DIRECTORY "nvm"


	typedef unsigned short ushort;
	typedef unsigned int uint;



	struct NavMeshMetadata {
		char header[12]; //12 bytes at start of file
		ushort nav_entries; //
		uint cell_entries;
		uint cell_extra_entries;
		uint region_link_entries;
		uint cell_link_entries;

	};
	//vec3 struct
	struct Vector3 {
		int x;
		int y;
		int z;
	};
	//vec2 struct

	struct Vector2 {
		int x;
		int y;
	};

	struct MountPoint{
		BYTE mount_point[6];
	};

	struct NavigationEntry {
		uint id;
		Vector3 position;
		ushort collision;
		float yaw;
		ushort unique_id;
		ushort unknown_0;
		ushort event_zone; // 0 = no collision while using CTF suit, 256 = collision when using CTF suit
		ushort region_id;
		ushort mount_point_entries;
		std::vector<MountPoint> mount_points;
	};

	struct NavigationCellLink {
		Vector2 min;
		Vector2 max;
		BYTE entry_count;
		std::vector<ushort> entry_index;
	};

	struct RegionLink {
		Vector2 min;
		Vector2 max;
		BYTE line_flag;
		BYTE line_source;
		BYTE line_destination;
		ushort source_cell;
		ushort destination_cell;
		ushort source_region;
		ushort destination_region;
	};

	struct NavigationCell {
		Vector2 min;
		Vector2 max;

		BYTE line_flag;
		BYTE line_source;
		BYTE line_destination;

		ushort source_cell;
		ushort destination_cell;
	};
#define TEXTURE_MAP_X 96
#define TEXTURE_MAP_Y 96

	struct TextureMap {
		
		ushort w1;
		ushort w2;
		ushort w3;
		ushort w4;
	};


#define HEIGHT_MAX_COUNT 97 * 97

	struct HeightMap {
		float Height;
	};


	struct NavMesh {
		NavMeshMetadata metadata;//std::unique_ptr<NavMeshMetadata> metadata;
		std::map<uint, NavigationEntry> nav_entries;
		std::vector<NavigationCell> nav_cells;
		std::vector<RegionLink> region_links;
		std::vector<NavigationCellLink> nav_cell_links;
		std::vector<TextureMap> texture_maps;
		std::vector<HeightMap> height_maps;
	};



}