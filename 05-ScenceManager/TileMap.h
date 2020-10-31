#pragma once
#include <vector>
#include "Sprites.h"
#include "Mario.h"

using namespace std;

#define CELL_SIZE 200

class TileMap
{
private:	
	// listSprites (ảnh) tương ứng vs các tiles
	vector<LPSPRITE> listSprites;
	// số tiles theo chiều ngang
	int numXTiles;
	// số tiles theo chiều dọc
	int numYTiles;
	// id để tạo texture map
	int id;

public:
	TileMap(int idTex, string txtMapPath);
	~TileMap();

	void Render(CMario* player);
};

