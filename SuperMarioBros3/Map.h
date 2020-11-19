#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "Sprites.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

class Map
{
private:
	int** TileMap;
	vector<LPSPRITE> Tiles; 
	LPDIRECT3DTEXTURE9 TileSet; //map1-1_bank.png
	int TotalColumnsOfMap, TotalRowsOfMap;
	int TotalColumnsOfTileSet, TotalRowsOfTileSet;
	int TotalTiles; //total tiles of tileset

	int MapWidth;
	int MapHeight;

	float CamX, CamY;

public:
	Map(int _idTileSet, int _totalRowsTileSet, int _totalColumnsTileSet, int _totalRowsMap, int _totalColumnsMap, int _totalTiles);
	~Map();
	void Draw(float x, float y);
	void Render();
	void ExtractTileFromTileSet();
	void LoadMap(LPCWSTR file_path);

	void SetCamPos(float x, float y) { CamX = x; CamY = y; }

	int GetTotalColumnsMap();
	int GetTotalRowsMap();
	int GetMapHeight();
	int GetMapWidth();
};