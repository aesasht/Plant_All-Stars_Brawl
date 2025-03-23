#ifndef _STATUS_BAR_H_
#define _STATUS_BAR_H_

#include "vector2.h"

class StatusBar
{
public:
	StatusBar() = default;
	~StatusBar() = default;
public:
	void set_hp(int hp)
	{
		this->hp = hp;
	}

	void set_mp(int mp)
	{
		this->mp = mp;
	}


private:
	int hp = 0;
	int mp = 0;
	


};





#endif // !_STATUS_BAR_H_
