#include "TileLayout.h"

FTileLayout::FTileLayout()
{
	HexagonMesh = nullptr;
	HasRiver = false;
	RiverConnections.SetNum(6);
}
