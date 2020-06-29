#pragma once
#include "Base.h"
#include "Layer.h"
BEGIN(Client)
class CTerrain;
class CPickingMgr : public CBase
{
	DECLARE_SINGLETON(CPickingMgr)
private:
	explicit CPickingMgr();
	virtual ~CPickingMgr() = default;

public:
	HRESULT	Ready_PickingMgr(CTerrain* pTerrain);
public:
	HRESULT	Get_Route(_float3 _WorldSrc, _float3 _WorldDst, vector<_float3>& _out);
	HRESULT Get_Route(_float3 _WorldSrc, POINT _ViewPortDst, vector<_float3>& _out);
	_bool	Get_WorldMousePos(POINT _pt, _float3* _vWorldMouse);
	CTerrain* Get_Terrain() { return m_pTerrain; }

private:
	CTerrain* m_pTerrain = nullptr;

public:
	virtual void Free() override;
};
END
