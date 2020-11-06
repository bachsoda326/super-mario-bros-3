#pragma once
#include <vector>
#include "Sprites.h"
#include "Mario.h"

using namespace std;

#define CELL_SIZE 150

class CTileMap
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
	CTileMap(int idTex, string txtMapPath);
	~CTileMap();

	void Render(CMario* player);
};

