#include "TileMap.h"
#include "Textures.h"
#include <fstream>
#include "Game.h"

TileMap::TileMap(int idTex, string txtMapPath)
{
	fstream fs;
	fs.open(txtMapPath);

	fs >> numXTiles;
	fs >> numYTiles;
	// Khởi tạo size cho listTiles
	vector<int> listTiles(numXTiles * numYTiles);
	// set các tile = các số trong file
	for (int i = 0; i < listTiles.size(); i++)
	{
		fs >> listTiles[i];
	}

	LPDIRECT3DTEXTURE9 texture = CTextures::GetInstance()->Get(idTex);
	D3DSURFACE_DESC desc;
	texture->GetLevelDesc(0, &desc);
	int numColumns = desc.Width / 16;	// 32
	//int numRows = desc.Height / 16;	// 76	

	LPSPRITE sprite;
	int tileIndex;
	int xIndex, yIndex, left, top;
	int xDrawIndex, yDrawIndex, xDraw, yDraw;

	for (int i = 0; i < listTiles.size(); i++)
	{
		tileIndex = listTiles[i];
		yIndex = tileIndex / numColumns;
		xIndex = tileIndex - yIndex * numColumns;
		left = xIndex * 16;
		top = yIndex * 16;
		// tạo sprite tương ứng vs các tile từ texture map
		sprite = new CSprite(i, left, top, left + 16, top + 16, texture);
		listSprites.push_back(sprite);
	}
}


TileMap::~TileMap()
{
}

void TileMap::Render(CMario* player)
{	
	CGame* game = CGame::GetInstance();
	float cx, cy;
	player->GetPosition(cx, cy);
	// tính vị trí topleft và botright của camera
	int xTopLeftCamera = cx - game->GetScreenWidth() / 2;
	int yTopLeftCamera = cy - game->GetScreenHeight() / 2;
	int xBotRightCamera = cx + game->GetScreenWidth() / 2;
	int yBotRightCamera = cy + game->GetScreenHeight() / 2;
	// tính vị trí topleft và botright của cell
	int xTopLeftCell = xTopLeftCamera / CELL_SIZE;
	int yTopLeftCell = yTopLeftCamera / CELL_SIZE;
	int xBotRightCell = xBotRightCamera / CELL_SIZE;
	int yBotRightCell = yBotRightCamera / CELL_SIZE;
	// tính vị trí topleft và botright của tile
	int xTopLeft = xTopLeftCell * CELL_SIZE / 16;
	int yTopLeft = yTopLeftCell * CELL_SIZE / 16;
	int xBotRight = ((xBotRightCell * CELL_SIZE) + CELL_SIZE) / 16;
	int yBotRight = ((yBotRightCell * CELL_SIZE) + CELL_SIZE) / 16;
	if (xBotRight >= numXTiles - 1)
		xBotRight = numXTiles - 1;
	if (yBotRight >= numYTiles - 1)
		yBotRight = numYTiles - 1;

	for (int x = xTopLeft; x <= xBotRight; x++)
	{
		for (int y = yTopLeft; y <= yBotRight; y++)
		{
			int xDrawIndex, yDrawIndex, xDraw, yDraw, i;
			i = x + y * numXTiles;
			yDrawIndex = i / numXTiles;
			xDrawIndex = i - yDrawIndex * numXTiles;
			xDraw = xDrawIndex * 16;
			yDraw = yDrawIndex * 16;

			//D3DXVECTOR2 trans = D3DXVECTOR2(floor(game->GetScreenWidth() / 2 - cx), floor(game->GetScreenHeight() / 2 - cy));
			listSprites[i]->Draw(xDraw, yDraw);
		}
	}

	/*int xDrawIndex, yDrawIndex, xDraw, yDraw;

	for (int i = 0; i < listSprites.size(); i++)
	{
		yDrawIndex = i / numXTiles;
		xDrawIndex = i - yDrawIndex*numXTiles;
		xDraw = xDrawIndex * 16;
		yDraw = yDrawIndex * 16;
		D3DXVECTOR2 trans = D3DXVECTOR2(floor(SCREEN_WIDTH / 2 - Camera::GetInstance()->GetPosition().x), floor(SCREEN_HEIGHT / 2 - Camera::GetInstance()->GetPosition().y));
		listSprites[i]->Draw(xDraw, yDraw, trans);
	}*/
}
