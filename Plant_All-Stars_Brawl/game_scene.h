#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"
#include <iostream>
#include "platform.h"
#include <vector>
#include "player.h"
#include "bullet.h"
extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;

extern bool is_debug;

extern Camera main_camera;
extern SceneManager scene_manager;
extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;

extern Player* player_1;
extern Player* player_2;


class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;
	void on_enter() {

		player_1->set_position(200, 50);
		player_2->set_position(975, 50);

		// 为了保障摄像机抖动不出现黑边，图片会比窗口尺寸大,这里的代码实现了居中显示功能
		pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

		pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;

		platform_list.resize(4);

		Platform& large_platform = platform_list[0];
		large_platform.img = &img_platform_large;
		large_platform.render_position = { 122, 455 };
		large_platform.shape.left = (float)large_platform.render_position.x + 30;
		large_platform.shape.right = (float)large_platform.render_position.x + img_platform_large.getwidth() - 30;
		large_platform.shape.y = (float)large_platform.render_position.y + 60;

		Platform& small_platform_1 = platform_list[1];
		small_platform_1.img = &img_platform_small;
		small_platform_1.render_position = { 175, 360 };
		small_platform_1.shape = {
			(float)small_platform_1.render_position.x + 40,
			(float)small_platform_1.render_position.x + img_platform_small.getwidth() - 40,
			(float)small_platform_1.render_position.y + img_platform_small.getheight() / 2
		};

		Platform& small_platform_2 = platform_list[2];
		small_platform_2.img = &img_platform_small;
		small_platform_2.render_position = { 855, 360 };
		small_platform_2.shape = {
			(float)small_platform_2.render_position.x + 40,
			(float)small_platform_2.render_position.x + img_platform_small.getwidth() - 40,
			(float)small_platform_2.render_position.y + img_platform_small.getheight() / 2
		};

		Platform& small_platform_3 = platform_list[3];
		small_platform_3.img = &img_platform_small;
		small_platform_3.render_position = { 515, 225 };
		small_platform_3.shape = {
			(float)small_platform_3.render_position.x + 40,
			(float)small_platform_3.render_position.x + img_platform_small.getwidth() - 40,
			(float)small_platform_3.render_position.y + img_platform_small.getheight() / 2
		};

	}
	void on_input(const ExMessage& msg) { 

		player_1->on_input(msg);
		player_2->on_input(msg);

		switch (msg.message)
		{
		case WM_KEYUP:
			// 'Q'
			if (msg.vkcode == 0x51)
				is_debug = !is_debug;
		}
		
	}
	void on_update(int delta)
	{
		player_1->on_update(delta);
		player_2->on_update(delta);

		main_camera.on_update(delta);

		/****/
		bullet_list.erase(
			std::remove_if(bullet_list.begin(), bullet_list.end(),
				[](const Bullet* bullet)
				{
					bool deletable = bullet->check_can_remove();
					if (deletable) delete bullet;
					return deletable;
				}
			), bullet_list.end()
		);


		for (Bullet* bullet : bullet_list)
			bullet->on_update(delta);
		
	}
	void on_draw(const Camera& camera)
	{
		putimage_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
		putimage_alpha(camera, pos_img_hills.x, pos_img_hills.y, &img_hills);

		// 绘制平台
		for (const Platform& platform : platform_list)
		{
			platform.on_draw(camera);
		}

		if(is_debug)
		{
			settextcolor(RGB(255, 0, 0));
			outtextxy(15, 15, _T("已开启调试模式，按 'Q' 键关闭"));
		}

		player_1->on_draw(camera);
		player_2->on_draw(camera);
		
		for (Bullet* bullet : bullet_list)
			bullet->on_draw(camera);

	}
	void on_exit()
	{

	}
private:
	POINT pos_img_sky = { 0 };			// 天空背景图位置
	POINT pos_img_hills = { 0 };		// 山脉背景图位置
};
#endif // !_GAME_SCENE_H_
