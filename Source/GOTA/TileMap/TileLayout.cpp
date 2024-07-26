#include "TileLayout.h"

FTileLayout::FTileLayout()
{
	HexagonMesh = nullptr;
	AllowBuilding = false;
	AllowNoBuilding = false;
	HasRiver = false;
	RiverConnections.SetNum(6);
}
