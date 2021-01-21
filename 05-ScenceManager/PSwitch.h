#pragma once
#include "GameObject.h"
// State
#define PSWITCH_STATE_NORMAL	100
#define PSWITCH_STATE_HIT		200
// Animation
#define PSWITCH_ANI_NORMAL		0
#define PSWITCH_ANI_HIT			1
// BBox
#define PSWITCH_BBOX_WIDTH		16
#define PSWITCH_BBOX_HEIGHT		16
// Others
#define PSWITCH_ANI_SET			36

class CPSwitch : public CGameObject
{
	void SetBoundingBox();
public:
	CPSwitch(float x, float y);

	void Render();
	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

