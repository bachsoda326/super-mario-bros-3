#pragma once
#include "GameObject.h"

#define CLOUD_TOOTH_BBOX_WIDTH  16
#define CLOUD_TOOTH_BBOX_HEIGHT 16

class CCloudTooth : public CGameObject
{
	void SetBoundingBox();
public:
	CCloudTooth();

	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

