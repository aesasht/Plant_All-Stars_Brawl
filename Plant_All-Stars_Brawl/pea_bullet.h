#ifndef _PEA_BULLET_H_
#define _PEA_BULLET_H_
#include "bullet.h"
#include "animation.h"
#include "util.h"
#include <graphics.h>

extern IMAGE img_pea;
extern Atlas atlas_pea_break;

class PeaBullet : public Bullet
{
public:
	PeaBullet() {
		size = { 64, 64 };
		damage = 10;

		animation_break.set_atlas(&atlas_pea_break);
		animation_break.set_interval(100);
		animation_break.set_loop(false);
		animation_break.set_callback([&]() { can_remove = true; });
	}


	~PeaBullet() = default;


	void on_collide()  
	{
		Bullet::on_collide();
		// ��������ӵ�������Ч
		switch (rand() % 3)
		{
		case 0: mciSendString(_T("play pea_break_1 form 0"), NULL, 0, NULL); break;
		case 1: mciSendString(_T("play pea_break_2 form 0"), NULL, 0, NULL); break;
		case 2: mciSendString(_T("play pea_break_2 form 0"), NULL, 0, NULL); break;
		}
	}

	void on_update(int delta)
	{
		position += velocity * (float)delta;

		if (!valid)
			animation_break.on_update(delta);

		if (check_if_exceeds_screen())
			can_remove = true;
	}

	void on_draw(const Camera& camera) const
	{
		if (valid)
			putimage_alpha(camera, (int)position.x, (int)position.y, &img_pea);
		else
			animation_break.on_draw(camera, (int)position.x, (int)position.y);
		Bullet::on_draw(camera);
	}

private:
	Animation animation_break;		// �㶹�ӵ����鶯��

};

#endif // !_PEA_BULLET_H_
