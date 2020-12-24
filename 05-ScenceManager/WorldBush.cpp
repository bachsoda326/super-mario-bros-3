#include "WorldBush.h"

void CWorldBush::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}
