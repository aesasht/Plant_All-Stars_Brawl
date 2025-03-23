#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"
#include <iostream>
#include "platform.h"
#include <vector>
#include "player.h"
#include "bullet.h"

#include "status_bar.h"

extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;

extern IMAGE img_avatar_peashooter; // 婉逗射手头像图片
extern IMAGE img_avatar_sunflower; // 龙日葵头像图片

extern IMAGE* img_player_1_avatar;
extern IMAGE* img_player_2_avatar;

extern IMAGE img_1P_winner; // 1P 获胜文本图片
extern IMAGE img_2P_winner; // 2P 获胜文本图片
extern IMAGE img_winner_bar; // 获胜玩家背景文本图片



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
		is_game_over = false;
		is_slide_out_started = false;
		// 计算结算图片的位置
		pos_img_winner_bar.x = -img_winner_bar.getwidth();
		pos_img_winner_bar.y = (getheight() - img_winner_bar.getheight()) / 2;
		pos_x_img_winner_bar_dst = (getwidth() - img_winner_bar.getwidth()) / 2;

		pos_img_winner_text.x = pos_img_winner_bar.x;
		pos_img_winner_text.y = (getheight() - img_1P_winner.getheight()) / 2;
		pos_x_img_winner_text_dst = (getwidth() - img_1P_winner.getwidth()) / 2;

		timer_winner_slide_in.restart();
		timer_winner_slide_in.set_wait_time(2500);
		timer_winner_slide_in.set_one_shot(true);
		timer_winner_slide_in.set_callback([&]() { is_slide_out_started = true; });

		timer_winner_slide_out.restart();
		timer_winner_slide_out.set_wait_time(1000);
		timer_winner_slide_out.set_one_shot(true);
		timer_winner_slide_out.set_callback([&]() { scene_manager.switch_to(SceneManager::SceneType::Menu); });

		mciSendString(_T("play bgm_game repeat from 0"), NULL, 0, NULL);

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

		status_bar_player_1.set_avater(img_player_1_avatar);
		status_bar_player_2.set_avater(img_player_2_avatar);

		status_bar_player_1.set_position(235, 625);
		status_bar_player_2.set_position(675, 625);

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

		const Vector2& position_player_1 = player_1->get_position();
		const Vector2& position_player_2 = player_2->get_position();

		//是否掉入窗口下方
		if (position_player_2.y >= getheight())
			player_1->set_hp(0);
		if (position_player_2.y >= getheight())
			player_2->set_hp(0);

		if (player_1->get_hp() <= 0 || player_2->get_hp() <= 0)
		{
			if (!is_game_over)
			{
				mciSendString(_T("stop bgm_game"), NULL, 0, NULL);
				mciSendString(_T("play ui_win from 0"), NULL, 0, NULL);
				if (player_1->get_hp() > 0)
					player_1->set_position(player_1->get_position().x, player_1->get_position().y - 250);
				else
					player_2->set_position(player_2->get_position().x, player_2->get_position().y - 250);
			}
			
			is_game_over = true;
			
		}

		status_bar_player_1.set_hp(player_1->get_hp());
		status_bar_player_1.set_mp(player_1->get_mp());

		status_bar_player_2.set_hp(player_2->get_hp());
		status_bar_player_2.set_mp(player_2->get_mp());

		if (is_game_over)
		{
			pos_img_winner_bar.x += (int)(speed_winner_bar * delta);
			pos_img_winner_text.x += (int)(speed_winner_bar * delta);

			if (!is_slide_out_started)
			{
				timer_winner_slide_in.on_update(delta);
				if (pos_img_winner_bar.x > pos_x_img_winner_bar_dst)
					pos_img_winner_bar.x = pos_x_img_winner_bar_dst;
				if (pos_img_winner_text.x > pos_x_img_winner_text_dst)
					pos_img_winner_text.x = pos_x_img_winner_text_dst;
			}
			else
				timer_winner_slide_out.on_update(delta);
			
		}


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

		if (is_game_over)
		{
			putimage_alpha(pos_img_winner_bar.x, pos_img_winner_bar.y, &img_winner_bar);
			putimage_alpha(pos_img_winner_text.x, pos_img_winner_text.y, player_1->get_hp() > 0 ? &img_1P_winner : &img_2P_winner);
		}
		else {
			status_bar_player_1.on_draw();
			status_bar_player_2.on_draw();
		}
	}
	void on_exit()
	{
		delete player_1; player_1 = nullptr;
		delete player_2; player_2 = nullptr;

		is_debug = false;

		bullet_list.clear();
		main_camera.reset();
		
	}
private:
	POINT pos_img_sky = { 0 };			// 天空背景图位置
	POINT pos_img_hills = { 0 };		// 山脉背景图位置
	StatusBar status_bar_player_1;		// 玩家1状态条
	StatusBar status_bar_player_2;		// 玩家2状态条
	bool is_game_over = false;			// 游戏是否结束

	POINT pos_img_winner_bar = { 0 };	// 结算动效背景位置
	POINT pos_img_winner_text = { 0 };	// 结算动效文本位置
	int pos_x_img_winner_bar_dst = 0;	// 结算动效背景移动的目标位置
	int pos_x_img_winner_text_dst = 0;	// 结算动效文本移动的目标位置

	Timer timer_winner_slide_in;		// 结算动效滑入定时器
	Timer timer_winner_slide_out;		// 结算动效滑出定时器
	bool is_slide_out_started = false;	// 结算动效是否开始滑出

	float speed_winner_bar = 3.0f;
	float speed_winner_text = 1.5f;
};
#endif // !_GAME_SCENE_H_
