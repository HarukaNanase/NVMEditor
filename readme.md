"#A try at a navmesh editor for Silkroad Online" 

In order to read and write Silkroad Online (hereafter named SRO), it was required to reverse engineer the binary structure of the .nvm files.


A Navmesh file consists of different parts:

#Navmesh Metadata

The navmesh metadata contains information about how many entries of each element exists inside of the navmesh.

The structure of metadata is the following:


	struct NavMeshMetadata 
	{
		char header[12]; // 12 bytes
		ushort nav_entries;
		uint cell_entries;
		uint cell_extra_entries;
		uint region_link_entries;
		uint cell_link_entries;

	};
  
  As we can see, there is a header (a string of 12 chars) and the number of entries for each type.
  
  
#Navigation Entry
  A navigation entry contains the position, as a vector, of the spot a yaw value that indicates the orientation of the box.
  It also contains a collision flag, an unique_id and some other information pertaining to the way that SRO maps are defined.
  
  The structure is the following:
  
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
	}
  
  
  Other structures can be found below:
  
  	struct MountPoint{
		BYTE mount_point[6];
	}
  
  	struct NavigationCell {
		Vector2 min;
		Vector2 max;
		BYTE entry_count;
		std::vector<ushort> entry_index;
	}

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
	}

	struct NavigationCellLink {
		Vector2 min;
		Vector2 max;

		BYTE line_flag;
		BYTE line_source;
		BYTE line_destination;

		ushort source_cell;
		ushort destination_cell;
	}
  
  	struct TextureMap {
		
		ushort w1;
		ushort w2;
		ushort w3;
		ushort w4;
	};
  
  	struct HeightMap {
		float Height;
	};
  
  
And finally the navmesh struct:

	struct NavMesh {
		NavMeshMetadata metadata;//std::unique_ptr<NavMeshMetadata> metadata;
		std::map<uint, NavigationEntry> nav_entries;
		std::vector<NavigationCell> nav_cells;
		std::vector<RegionLink> region_links;
		std::vector<NavigationCellLink> nav_cell_links;
		std::vector<TextureMap> texture_maps;
		std::vector<HeightMap> height_maps;
	};

  
  
