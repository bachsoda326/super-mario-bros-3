#include "TileMap.h"
#include "Textures.h"
#include <fstream>
#include "Game.h"

CTileMap::CTileMap(int idTex, string txtMapPath)
{
	id = idTex;

	fstream fs;
	fs.open(txtMapPath);

	fs >> numXTiles;
	fs >> numYTiles;
	// Init size for listTiles
	vector<int> listTiles(numXTiles * numYTiles);
	// Set tile = num in txt file
	for (int i = 0; i < listTiles.size(); i++)
	{
		fs >> listTiles[i];
	}

	LPDIRECT3DTEXTURE9 texture = CTextures::GetInstance()->Get(idTex);
	D3DSURFACE_DESC desc;
	texture->GetLevelDesc(0, &desc);
	int numColumns = desc.Width / TILE_SIZE;	// 32
	//int numRows = desc.Height / TILE_SIZE;	// 76	

	LPSPRITE sprite;
	int tileIndex;
	int xIndex, yIndex, left, top;
	int xDrawIndex, yDrawIndex, xDraw, yDraw;

	for (int i = 0; i < listTiles.size(); i++)
	{
		tileIndex = listTiles[i];
		yIndex = tileIndex / numColumns;
		xIndex = tileIndex - yIndex * numColumns;
		left = xIndex * TILE_SIZE;
		top = yIndex * TILE_SIZE;
		// Create sprite with tiles from texture map
		sprite = new CSprite(i, left, top, left + TILE_SIZE, top + TILE_SIZE, 0, 0, texture);
		listSprites.push_back(sprite);
	}
}


CTileMap::~CTileMap()
{
}

void CTileMap::Render()
{	
	CGame* game = CGame::GetInstance();
	float cx, cy;
	/*player->GetPosition(cx, cy);*/
	CCamera::GetInstance()->GetPosition();
	cx = CCamera::GetInstance()->GetPosition().x;
	cy = CCamera::GetInstance()->GetPosition().y;
	// Cal topleft and botright of camera
	int xTopLeftCamera = cx - game->GetScreenWidth() / 2;
	int yTopLeftCamera = cy - game->GetScreenHeight() / 2;
	int xBotRightCamera = cx + game->GetScreenWidth() / 2;
	int yBotRightCamera = cy + game->GetScreenHeight() / 2;
	// Cal topleft and botright of cell
	int xTopLeftCell = xTopLeftCamera / CELL_SIZE;
	int yTopLeftCell = yTopLeftCamera / CELL_SIZE;
	int xBotRightCell = xBotRightCamera / CELL_SIZE;
	int yBotRightCell = yBotRightCamera / CELL_SIZE;
	// Cal topleft and botright of tile
	int xTopLeft = xTopLeftCell * CELL_SIZE / TILE_SIZE;
	int yTopLeft = yTopLeftCell * CELL_SIZE / TILE_SIZE;
	int xBotRight = ((xBotRightCell * CELL_SIZE) + CELL_SIZE) / TILE_SIZE;
	int yBotRight = ((yBotRightCell * CELL_SIZE) + CELL_SIZE) / TILE_SIZE;
	if (xBotRight >= numXTiles - 1)
		xBotRight = numXTiles - 1;
	if (yBotRight >= numYTiles - 1)
		yBotRight = numYTiles - 1;

	if (xTopLeft < 0)
		xTopLeft = 0;
	if (yTopLeft < 0)
		yTopLeft = 0;

	for (int x = xTopLeft; x <= xBotRight; x++)
	{
		for (int y = yTopLeft; y <= yBotRight; y++)
		{
			int xDrawIndex, yDrawIndex, xDraw, yDraw, i;
			i = x + y * numXTiles;
			yDrawIndex = i / numXTiles;
			xDrawIndex = i - yDrawIndex * numXTiles;
			xDraw = xDrawIndex * TILE_SIZE;
			yDraw = yDrawIndex * TILE_SIZE;

			listSprites[i]->Draw(xDraw, yDraw);
		}
	}	
}
