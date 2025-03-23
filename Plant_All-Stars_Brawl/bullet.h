#ifndef _BULLET_H_
#define _BULLET_H_

#include "Vector2.h"
#include "player_id.h"
#include "camera.h"
#include <functional>
#include <graphics.h>

extern bool is_debug;

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;
public:
	void set_damage(int damage) {
		this->damage = damage;
	}

	int get_damage() const
	{
		return this->damage;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	void set_velocity(float x, float y)
	{
		velocity = { x, y };
	}

	void set_collide_target(PlayerID target)
	{
		target_id = target;
	}

	PlayerID get_collide_target() const
	{
		return target_id;
	}

	void set_position(float x, float y)
	{
		this->position = { x, y };
	}

	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}

	void set_valid(bool flag)
	{
		this->valid = flag;
	}

	bool get_valid() const
	{
		return this->valid;
	}

	bool check_can_remove() const
	{
		return this->can_remove;
	}

	virtual void on_collide()
	{
		if (callback)
			callback();
	}

	virtual bool check_collision(const Vector2& position, const Vector2& size)
	{
		return this->position.x + this->size.x / 2 >= position.x
			&& this->position.x + this->size.x / 2 <= position.x + size.x
			&& this->position.y + this->size.y / 2 >= position.y
			&& this->position.y + this->size.y / 2 <= position.y + size.y;
	}

	virtual void on_update(int delta) { }

	virtual void on_draw(const Camera& camera) const {
		if (is_debug) {
			setfillcolor(RGB(255, 255, 255));
			if (valid)
				setlinecolor(RGB(255, 255, 255));
			else 
				setlinecolor((RGB(0, 0, 0)));
			rectangle((int)position.x, (int)position.y, (int)(position.x + size.x), (int)(position.y + size.y));
			solidcircle((int)(position.x + size.x / 2), (int)(position.y + size.y / 2), 5);
		}
	
	}

protected:
	// ����ӵ��Ƿ�ɳ���Ļ��
	bool check_if_exceeds_screen()
	{
		return (position.x + size.x <= 0 || position.x >= getwidth() || position.y + size.y <= 0 || position.y >= getheight());
	}

protected:
	Vector2 size;				// �ӵ��ߴ�
	Vector2 position;			// �ӵ�λ��
	Vector2 velocity;			// �ӵ��ٶ�
	int damage = 10;			// �ӵ��˺�

	bool valid = true;			// �ӵ��Ƿ���Ч
	bool can_remove = false;	// �Ƿ���Ա��Ƴ�

	std::function<void()> callback;	// �ӵ���ײ�ص�����
	
	PlayerID target_id = PlayerID::P1;

};

#endif // !_BULLET_H_
