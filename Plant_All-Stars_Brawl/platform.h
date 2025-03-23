#ifndef _PLATFORM_H_
#define _PLATFORM_H_
#include <graphics.h>
#include "camera.h" 
#include "util.h"

extern bool is_debug;

class Platform
{
public:
	struct CollisionShape
	{
		float left, right;	// 水平方向上线段的起点和终点坐标
		float y;			// 平台的竖直坐标位置
	};
public:
	CollisionShape shape;
	IMAGE* img = nullptr;
	POINT render_position = { 0 };
public:
	Platform() = default;
	~Platform() = default;

	void on_draw(const Camera& camera) const
	{
		putimage_alpha(camera, render_position.x, render_position.y, img);
		if (is_debug)
		{
			setlinecolor(RGB(255, 0, 0));
			line(camera, (int)shape.left, (int)shape.y, (int)shape.right, (int)shape.y);
		}
	}

};

#endif // !_PLATFORM_H_
