#ifndef _SCENE_H_
#define _SCENE_H_
#include <graphics.h>

#include "camera.h"

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	virtual void on_enter() { }							// 处理场景进入时的初始化逻辑

	/* 对应游戏主循环框架的三个阶段 */
	virtual void on_input(const ExMessage& msg) { }		// 处理玩家输入时调用
	virtual void on_update(int delta) { }						// 处理游戏逻辑更新时调用
	virtual void on_draw(const Camera& camera) { }							// 画面绘制时调用
	

	virtual void on_exit() { }							// 场景退出时的卸载逻辑

private:

};

#endif // !_SECENE_H_
