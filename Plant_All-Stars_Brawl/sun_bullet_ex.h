#ifndef _SUN_BULLET_EX_H_
#define _SUN_BULLET_EX_H_
#include "bullet.h"
#include "animation.h"
#include "util.h"
#include <graphics.h>


extern Atlas atlas_sun_ex; // 特殊日光动画图集
extern Atlas atlas_sun_ex_explode; // 特殊日光爆炸动画图集

extern Camera main_camera;

class SunBulletEx : public Bullet
{
public:
	SunBulletEx()
	{
		size = { 288, 288 };
		damage = 50;

		animation_idle.set_atlas(&atlas_sun_ex);
		animation_idle.set_interval(50);
		animation_idle.set_loop(true);

		animation_explode.set_atlas(&atlas_sun_ex_explode);
		animation_explode.set_interval(50);
		animation_explode.set_loop(false);
		animation_explode.set_callback([&]() { can_remove = true; });

		IMAGE* frame_sun = animation_idle.get_frame();
		IMAGE* frame_explode = animation_explode.get_frame();

		explode_render_offset = {
			(frame_sun->getwidth() - frame_explode->getwidth()) / 2.0f,
			(frame_sun->getheight() - frame_explode->getheight()) / 2.0f,
		};


	}
	~SunBulletEx() = default;

	void on_collide()
	{
		Bullet::on_collide();

		// 播放太阳花子弹爆炸音乐
		mciSendString(_T("play sun_explode_ex form 0"), NULL, 0, NULL);
		// 爆炸时的屏幕抖动效果
		main_camera.shake(20, 400);
	}

	void on_update(int delta)
	{
		if (valid)
		{
			position += velocity * (float)delta;
		}

		if (valid)
		{
			animation_idle.on_update(delta);
		}
		else
		{
			animation_explode.on_update(delta);
		}

		if (check_if_exceeds_screen())
		{
			can_remove = true;
		}

	}

	void on_draw(const Camera& camera) const
	{
		if (valid)
			animation_idle.on_draw(camera, (int)position.x, (int)position.y);
		else
		{
			animation_explode.on_draw(
				camera,
				(int)(position.x + explode_render_offset.x),
				(int)(position.x + explode_render_offset.y)
			);
		}
		Bullet::on_draw(camera);
	}



	bool check_collision(const Vector2& position, const Vector2& size)
	{
		bool is_collide_x =
			(
				max(this->position.x + this->size.x, position.x + size.x)
				- min(this->position.x, position.x) <= this->size.x + size.x
			);
		bool is_collide_y =
			(
				max(this->position.y + this->size.y, position.y + size.y)
				- min(this->position.y + position.y, position.y) <= this->size.y + size.y 
				);
		return is_collide_x && is_collide_y;
	}
private:
	const float gravity = 1e-3f;

private:
	Animation animation_idle;		// 巨大日光炸弹默认动画
	Animation animation_explode;	// 巨大日光炸弹爆炸动画
	Vector2 explode_render_offset;	// 爆炸动画渲染偏移
};




#endif // !_SUN_BULLET_EX_H_
