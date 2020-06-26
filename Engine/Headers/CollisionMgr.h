#pragma once

// ȭ�鿡 �׷����� ��ü������ ��Ƴ��� ������ ���� �������� ���ش�.

#include "Base.h"

BEGIN(Engine)

class CGameObject;
class  ENGINE_DLL CCollisionMgr final : public CBase
{
	DECLARE_SINGLETON(CCollisionMgr)
public:
	enum COLLISION_GROUP { COL_RECT, COL_BOX,COL_END };
private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr() = default;
public:
	HRESULT Ready_Component_Prototype();
	HRESULT Ready_Component(void* pArg);
public:
	HRESULT Add_CollisionGroup(COLLISION_GROUP eGroup, CGameObject* pGameObject);
	HRESULT CheckCollision();

private:
	HRESULT	CheckCollision_Box();


private:
	vector<CGameObject*>			m_CollisionGroup[COL_END];
	typedef vector<CGameObject*>	COLLISIONVECTOR;


public:
	virtual void Free();
};

END