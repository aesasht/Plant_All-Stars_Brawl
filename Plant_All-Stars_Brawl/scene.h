#ifndef _SCENE_H_
#define _SCENE_H_
#include <graphics.h>

#include "camera.h"

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	virtual void on_enter() { }							// ����������ʱ�ĳ�ʼ���߼�

	/* ��Ӧ��Ϸ��ѭ����ܵ������׶� */
	virtual void on_input(const ExMessage& msg) { }		// �����������ʱ����
	virtual void on_update(int delta) { }						// ������Ϸ�߼�����ʱ����
	virtual void on_draw(const Camera& camera) { }							// �������ʱ����
	

	virtual void on_exit() { }							// �����˳�ʱ��ж���߼�

private:

};

#endif // !_SECENE_H_
