
#include <fstream>
#include <iostream>
#include "Map.h"
#include "Textures.h"

Map::Map(int _idTileSet, int _totalRowsTileSet, int _totalColumnsTileSet, int _totalRowsMap, int _totalColumnsMap, int _totalTiles)
{
	this->TileSet = CTextures::GetInstance()->Get(_idTileSet);
	this->TotalRowsOfTileSet = _totalRowsTileSet;
	this->TotalColumnsOfTileSet = _totalColumnsTileSet;
	this->TotalTiles = _totalTiles;
	this->TotalRowsOfMap = _totalRowsMap;
	this->TotalColumnsOfMap = _totalColumnsMap;
	this->MapWidth = this->MapHeight = 0;
}

Map::~Map()
{
	if (TileMap)
	{
		for (int i = 0; i < TotalRowsOfMap; i++)
		{
			delete TileMap[i];
		}
		delete TileMap;
		TileMap = nullptr;
	}
}

void Map::Render()
{
	for (int r = 0; r < TotalRowsOfMap; r++)
		for (int c = 0; c < TotalColumnsOfMap; c++)
		{
			Tiles[TileMap[r][c] - 1]->Draw(c * TILE_WIDTH, r * TILE_HEIGHT, 255);
		}
}

void Map::ExtractTileFromTileSet()
{
	for (int TileNumber = 0; TileNumber < TotalTiles; TileNumber++)
	{
		int left = TileNumber / TotalColumnsOfTileSet * TILE_WIDTH;
		int top = TileNumber % TotalRowsOfTileSet * TILE_HEIGHT;
		int right = left + TILE_WIDTH;
		int bottom = top + TILE_HEIGHT;
		LPSPRITE newTile = new CSprite(TileNumber, left, top, right, bottom, TileSet);
		this->Tiles.push_back(newTile);
	}
}

void Map::LoadMap(LPCWSTR path)
{
	ifstream ifs;

	ifs.open(path); //
	
	this->TileMap = new int* [TotalRowsOfMap];
	for (int i = 0; i < TotalRowsOfMap; i++)
	{
		TileMap[i] = new int[TotalColumnsOfMap];
		for (int j = 0; j < TotalColumnsOfMap; j++)
		{
			ifs >> TileMap[i][j];
		}
	}
	ifs.close();
}

int Map::GetMapWidth()
{
	return TotalColumnsOfMap * TILE_WIDTH;
}

int Map::GetMapHeight()
{
	return TotalRowsOfMap * TILE_HEIGHT;
}


