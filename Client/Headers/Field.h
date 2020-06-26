#pragma once
#include "GameObject.h"

#include "Management.h"
#include "Client_Defines.h"

BEGIN(Client)
#define BRICK_MARGIN 0.05f
class CWall;
class CTrigger;
class CField : public CGameObject
{
public:
	enum WALL { WALL_LEFT, WALL_RIGHT, WALL_FRONT, WALL_BACK, WALL_TOP, WALL_BOTTOM, WALL_END};
public:
	typedef struct tagStatedesc
	{
		SCENEID		eSceneID = SCENE_END;
		_uint		iBrickNumX = 0;
		_uint		iBrickNumY = 0;
		_uint		iBrickNumZ = 0;
		//어느 벽에 붙을건지
		WALL		eWhereGoal = WALL_END;
		//그 벽의 중점에서 떨어진 정도
		_uint		iGoalX = 0;
		_uint		iGoalY = 0;
	}STATEDESC;
protected:
	explicit CField(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CField(const CField& rhs);
	virtual ~CField() = default;


public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int Late_Update_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	CWall**	Get_Walls();


private:
	CTransform*	m_pTransform = nullptr;

private:
	STATEDESC	m_tDesc;
	CWall*	m_Walls[6];
	CTrigger*	m_pGoal = nullptr;
public:
	static CField* Create(PDIRECT3DDEVICE9 pGraphic_Device);
	virtual CField* Clone_GameObject(void* pArg);
	virtual void Free();


};
END
