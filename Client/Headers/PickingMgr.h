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
	_int	Clear_PickingMgr();

	HRESULT	Add_Interactable(CInteractable* pObj);
public:
	HRESULT	Get_Route(_float3 _WorldSrc, _float3 _WorldDst, vector<_float3>& _out);
	HRESULT Get_Route(_float3 _WorldSrc, POINT _ViewPortDst, vector<_float3>& _out);
	_bool	Get_WorldMousePos(POINT _pt, _float3* _vWorldMouse);

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
