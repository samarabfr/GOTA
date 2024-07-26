#include "TileLayout.h"

FTileLayout::FTileLayout()
{
	HexagonMesh = nullptr;
	AllowBuilding = false;
	AllowNoBuilding = false;
	AllowRiver = false;
	RiverConnections.SetNum(6);
}
