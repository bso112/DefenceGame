#pragma once
#include "Base.h"
#include "Layer.h"



BEGIN(Client)
class CTerrain;
class CInteractable;
class CPickingMgr : public CBase
{
	DECLARE_SINGLETON(CPickingMgr)
private:
	explicit CPickingMgr();
	virtual ~CPickingMgr() = default;

public:
	HRESULT	Ready_PickingMgr(CTerrain* pTerrain);

	HRESULT	Register_Observer(CInteractable* pObj);
	HRESULT	UnRegister_Observer(CInteractable* pObj);
public:
	HRESULT	Get_Route(_float3 _WorldSrc, _float3 _WorldDst, vector<_float3>& _out);
	HRESULT Get_Route(_float3 _WorldSrc, POINT _ViewPortDst, vector<_float3>& _out);
	_bool	Get_WorldMousePos(POINT _pt, _float3* _vWorldMouse);

public:
	//vPos 를 기준으로 _fRadius 범위안에 있는 게임오브젝트들을 반환한다.
	vector<CGameObject*> OverlapSphere(_float3 vPos, _float _fRadius);

	CTerrain* Get_Terrain() { return m_pTerrain; }
	HRESULT	Pick_Object(POINT _ViewPortPoint, _float3* pHitPos);

public:
	virtual HRESULT	OnKeyDown(_int KeyCode);

private:
	CTerrain*					m_pTerrain = nullptr;
	list<CInteractable*>		m_listObject;
	CInteractable*				m_pFocus = nullptr;
public:
	virtual void Free() override;
};
END
