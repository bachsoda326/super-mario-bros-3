#include "Grid.h"
#include "Constants.h"
#include "WarpPipe.h"

CGrid::CGrid(int width, int height, int cellSize) : width(width), height(height), cellSize(cellSize)
{
	numXCells = ceil((float)this->width / cellSize);
	numYCells = ceil((float)this->height / cellSize);

	// resize lại listCells
	listCells.resize(numXCells * numYCells);
}

CGrid::~CGrid()
{
}

void CGrid::AddObjToCell(CGameObject* obj)
{
	float x, y;
	obj->GetPosition(x, y);
	Cell& cell = GetCell(x, y);
	cell.listObj.push_back(obj);
}

void CGrid::AddObjToCell(int cellIndex, CGameObject* obj)
{
	Cell& cell = listCells[cellIndex];
	cell.listObj.push_back(obj);
}

Cell& CGrid::GetCell(int x, int y)
{
	if (x < 0) x = 0;
	if (x >= numXCells) x = numXCells;
	if (y < 0) y = 0;
	if (y >= numYCells) y = numYCells;

	return listCells[x + y * numXCells];
}

Cell& CGrid::GetCell(D3DXVECTOR3& posObj)
{
	int cellX = posObj.x / cellSize;
	int cellY = posObj.y / cellSize;

	return GetCell(cellX, cellY);
}

void CGrid::CalcColliableObjs(CCamera* camera, vector<LPGAMEOBJECT>& objs, vector<LPGAMEOBJECT>& afterObjs)
{
	// tính vị trí topleft và botright của camera
	int xTopLeftCamera = camera->GetPosition().x - CGame::GetInstance()->GetScreenWidth() / 2;
	int yTopLeftCamera = camera->GetPosition().y - CGame::GetInstance()->GetScreenHeight() / 2;
	int xBotRightCamera = camera->GetPosition().x + CGame::GetInstance()->GetScreenWidth() / 2;
	int yBotRightCamera = camera->GetPosition().y + CGame::GetInstance()->GetScreenHeight() / 2;
	// tính vị trí topleft và botright của cell
	int xTopLeftCell = xTopLeftCamera / cellSize;
	int yTopLeftCell = yTopLeftCamera / cellSize;
	int xBotRightCell = xBotRightCamera / cellSize;
	int yBotRightCell = yBotRightCamera / cellSize;
	/*DebugOut(L"[KT] Camera x topleft: %d\n", xTopLeftCell);
	DebugOut(L"[KT] Camera y topleft: %d\n", yTopLeftCell);
	DebugOut(L"[KT] Camera x botright: %d\n", xBotRightCell);
	DebugOut(L"[KT] Camera y botright: %d\n", yBotRightCell);*/

	for (int x = xTopLeftCell; x <= xBotRightCell; x++)
	{
		for (int y = yTopLeftCell; y <= yBotRightCell; y++)
		{
			Cell colliableCell = GetCell(x, y);

			for (int i = 0; i < colliableCell.listObj.size(); i++)
			{
				// If WarpPipe then add to afterObjs (for render front Mario)
				if (dynamic_cast<CWarpPipe*>(colliableCell.listObj[i]))
				{
					if (afterObjs.size() == 0)
						afterObjs.push_back(colliableCell.listObj[i]);
					else
					{
						// Check if already have that obj in list
						for (int j = 0; j < afterObjs.size(); j++)
						{
							if (colliableCell.listObj[i] == afterObjs[j])
								break;
							else if (j == afterObjs.size() - 1)
								afterObjs.push_back(colliableCell.listObj[i]);
						}
					}
				}
				// Else then add to objs
				else
				{
					if (objs.size() == 0)
						objs.push_back(colliableCell.listObj[i]);
					else
					{
						// Check if already have that obj in list
						for (int j = 0; j < objs.size(); j++)
						{
							if (colliableCell.listObj[i] == objs[j])
								break;
							else if (j == objs.size() - 1)
								objs.push_back(colliableCell.listObj[i]);
						}
					}
				}				
			}
		}
	}
}
