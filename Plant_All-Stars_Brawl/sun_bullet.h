#ifndef _SUN_BULLET_H_
#define _SUN_BULLET_H_
#include "bullet.h"
#include "animation.h"
#include "util.h"
#include <graphics.h>

extern Atlas atlas_sun; // �չ⶯��ͼ��
extern Atlas atlas_sun_explode; // �չⱬը����ͼ��
extern Camera main_camera;

class SunBullet : public Bullet
{
public:
	SunBullet()
	{
		size = { 96, 96 };
		damage = 30;

		animation_sun.set_atlas(&atlas_sun);
		animation_sun.set_interval(50);
		animation_sun.set_loop(true);
		
		animation_sun_explode.set_atlas(&atlas_sun_explode);
		animation_sun_explode.set_interval(50);
		animation_sun_explode.set_loop(false);
		animation_sun_explode.set_callback([&]() { can_remove = true; });

		IMAGE* frame_sun = animation_sun.get_frame();
		IMAGE* frame_explode = animation_sun_explode.get_frame();

		explode_render_offset = {
			(frame_sun->getwidth() - frame_explode->getwidth()) / 2.0f,
			(frame_sun->getheight() - frame_explode->getheight()) / 2.0f,
		};
	}
	~SunBullet() = default;

	void on_collide()
	{
		Bullet::on_collide();

		// ����̫�����ӵ���ը����
		mciSendString(_T("play sun_explode form 0"), NULL, 0, NULL);
		// ��ըʱ����Ļ����Ч��
		main_camera.shake(5, 250);
	}

	void on_update(int delta)
	{
		if (valid)
		{
			velocity.y += gravity * delta;
			position += velocity * (float)delta;
		}

		if (valid)
		{
			animation_sun.on_update(delta);
		}
		else
		{
			animation_sun_explode.on_update(delta);
		}

		if (check_if_exceeds_screen())
		{
			can_remove = true;
		}
		
	}

	void on_draw(const Camera& camera) const
	{
		if (valid)
			animation_sun.on_draw(camera, (int)position.x, (int)position.y);
		else
			animation_sun_explode.on_draw(
				camera, 
				(int)position.x + explode_render_offset.x, 
				(int)position.y + explode_render_offset.y); 
		Bullet::on_draw(camera);
	}

private:
	const float gravity = 1e-3f;

private:
	Animation animation_sun;			// ̫������
	Animation animation_sun_explode;	// �չⱬը
	Vector2 explode_render_offset;		// ��ը������Ⱦƫ��

};


#endif // !_SUN_BULLET_H_
