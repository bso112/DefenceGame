#pragma once

// 화면에 그려야할 객체들응ㄹ 모아놓고 순서에 따라 렌더콜을 해준다.

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