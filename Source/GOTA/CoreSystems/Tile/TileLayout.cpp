#include "TileLayout.h"

FTileLayout::FTileLayout()
{
	HexagonMesh = nullptr;
	AllowNativesBuilding = false;
	AllowColonistBuilding = false;
	AllowNoBuilding = false;
	HasRiver = false;
	RiverConnections.SetNum(6);
}
