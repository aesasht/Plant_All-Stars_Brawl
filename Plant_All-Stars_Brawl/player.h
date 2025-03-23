#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "animation.h"
#include "camera.h"
#include "player_id.h"
#include "platform.h"
#include "bullet.h"
#include <vector>
#include <graphics.h>

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;
extern bool is_debug;

class Player
{
public:
	Player() {
		current_animation = &animation_idle_right;

		// 初始化技能冷却定时器
		timer_attack_cd.set_wait_time(attack_cd);
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]()
			{
				can_attack = true;
			}
		);
		timer_invulnerable.set_wait_time(750);
		timer_invulnerable.set_one_shot(true);
		timer_invulnerable.set_callback([&]()
			{
				is_invulnerable = false;
			});

		timer_invulnerable_blink.set_wait_time(75);
		timer_invulnerable_blink.set_callback(
			[&]()
			{
				is_showing_sketch_frame = !is_showing_sketch_frame;
			}

		);
		
	}
	~Player() = default;

	virtual void on_update(int delta)			
	{
		// 判断玩家当前移动方向
		int direction = is_right_key_down - is_left_key_down;

		if (direction != 0)
		{
			if(!is_attacking_ex)
				is_facing_right = direction > 0;
			current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
			float distance = direction * run_velocity * delta;
			on_run(distance);

		}
		else
		{
			current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
		}
		if (is_attacking_ex)
			current_animation = is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left;
			
		current_animation->on_update(delta);
		timer_attack_cd.on_update(delta);

		timer_invulnerable.on_update(delta);
		timer_invulnerable_blink.on_update(delta);

		if (is_showing_sketch_frame)
		{
			sketch_image(current_animation->get_frame(), &img_sketch);
		}

		move_and_collide(delta);
	}
	virtual void on_draw(const Camera& camera)	
	{
		if (hp > 0 && is_invulnerable && is_showing_sketch_frame)
			putimage_alpha(camera, (int)position.x, (int)position.y, &img_sketch);
		else
			current_animation->on_draw(camera, (int)position.x, (int)position.y);

		if (is_debug)
		{
			setlinecolor(RGB(0, 125, 255));
			rectangle((int)position.x, (int)position.y, (int)(position.x + size.x), (int)(position.y + size.y));
		}


	}
	virtual void on_input(const ExMessage& msg) 
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
					// 'A'
				case 0x41:
					is_left_key_down = true;
					break;
					// 'Q'
				case 0x44:
					is_right_key_down = true;
					break;
					// 'W'
				case 0x57:
					on_jump();
					break;
					// 'F'
				case 0x46:
					if (can_attack)
					{
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();
					}
					break;
					// 'G'
				case 0x47:
					if (mp >= 100)
					{
						on_attack_ex();
						mp = 0;
					}
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
					// '←'
				case VK_LEFT:
					is_left_key_down = true;
					break;
					// '→'
				case VK_RIGHT:
					is_right_key_down = true;
					break;
					// '↑'
				case VK_UP:
					on_jump();
					break;
					// '.'
				case VK_OEM_PERIOD:
					if (can_attack)
					{
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();
					}
					break;
					// '/'
				case VK_OEM_2:
					if (mp >= 100)
					{
						on_attack_ex();
						mp = 0;
					}
					break;
				}
				break;
			default:
				break;
			}
			break;
		case WM_KEYUP:
			switch (id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
					// 'A'
				case 0x41:
					is_left_key_down = false;
					break;
					// 'Q'
				case 0x44:
					is_right_key_down = false;
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
					// '←'
				case VK_LEFT:
					is_left_key_down = false;
					break;
					// '→'
				case VK_RIGHT:
					is_right_key_down = false;
					break;

				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	virtual void on_run(float distance)
	{
		if (is_attacking_ex)
			return;

		position = {
			position.x += distance,
			position.y
		};
	}

	virtual void on_jump()
	{
		if (is_attacking_ex)
			return;

		if (velocity.y != 0)
			return;
		velocity.y += jump_velocity;
	}

	// 普通攻击
	virtual void on_attack() { }
	// 技能攻击
	virtual void on_attack_ex() { }
	
	void make_invulnerable()
	{
		is_invulnerable = true;
		timer_invulnerable.restart();
	}


	void set_id(PlayerID id)
	{
		this->id = id;
	}

	void set_position(float x, float y)
	{
		position = { x, y };
	}

	const Vector2& get_position() const
	{
		return position;
	}
	const Vector2& get_size() const
	{
		return size;
	}

protected:
	const float run_velocity = 0.55f;	// 玩家奔跑速度
	const float gravity = 1.6e-3f;		// 玩家重力
	const float jump_velocity = -0.85f;	// 跳跃速度

protected:
	int mp = 0;
	int hp = 100;

	Vector2 size;					// 角色尺寸
	Vector2 position;				// 角色位置
	Vector2 velocity;				// 角色速度

	Animation* current_animation;	// 当前动画
	Animation animation_idle_left;	// 朝向左的默认动画
	Animation animation_idle_right;	// 朝向右的默认动画

	Animation animation_run_left;	// 朝向左的奔跑动画
	Animation animation_run_right;	// 朝向右的奔跑动画

	Animation animation_attack_ex_right;	// 向右特殊攻击动画
	Animation animation_attack_ex_left;		// 向左特殊攻击动画

	PlayerID id = PlayerID::P1;

	bool is_left_key_down = false;	// 向左移动按键是否按下
	bool is_right_key_down = false;	// 向右移动案件是否按下

	bool is_facing_right = true;	// 角色是否朝向右方
	bool can_attack = true;
	int attack_cd = 500;			// 普通攻击冷却时间
	bool can_attack_cd = true;		// 是否可以释放普通攻击
	Timer timer_attack_cd;			// 普通攻击冷却定时器

	bool is_attacking_ex = false;	// 当前是否正在播放特殊攻击

	IMAGE img_sketch;						// 动画帧剪影图片
	bool is_invulnerable = false;			// 角色是否处于无敌状态
	bool is_showing_sketch_frame = false;	// 但前帧是否应该显示剪影
	Timer timer_invulnerable;				// 无敌状态定时器
	Timer timer_invulnerable_blink;			// 无敌状态闪闪定时器



protected:
	void move_and_collide(int delta)
	{
		velocity.y += gravity * delta;
		position += velocity * (float)delta;

		// 玩家只有在下坠时才会和平台进行碰撞
		if (velocity.y > 0)
		{
			for (const Platform& platform : platform_list)
			{
				const Platform::CollisionShape& shape = platform.shape;
				bool is_collide_x = (
					max(position.x + size.x, shape.right) - min(position.x, shape.left) <=
					size.x + (shape.right - shape.left)
				);
				bool is_collide_y = (shape.y >= position.y && shape.y <= position.y + size.y);

				if (is_collide_x && is_collide_y)
				{	
					// 上一帧竖直方向的位置
					float delta_pos_y = velocity.y * delta;
					float last_tick_foot_pos_y = position.y + size.y - delta_pos_y;
					// 这一帧玩家脚底坐标穿过了平台而上一帧玩家脚底位置在平台之上
					if (last_tick_foot_pos_y <= shape.y)
					{
						position.y = shape.y - size.y;
						velocity.y = 0;

						break;
					}
				}

			}
		}
		if (!is_invulnerable) {
			for (Bullet* bullet : bullet_list)
			{
				if (!bullet->get_valid() || bullet->get_collide_target() != id)
					continue;
				if (bullet->check_collision(position, size))
				{
					make_invulnerable();
					bullet->on_collide();
					bullet->set_valid(false);
					hp -= bullet->get_damage();
				}
			}
		}
	}
};

#endif // !_PLAYER_H_
