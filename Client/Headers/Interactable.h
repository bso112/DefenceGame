#pragma once
#include "GameObject.h"
BEGIN(Client)
class CInteractable abstract: public CGameObject
{
protected:
	CInteractable(PDIRECT3DDEVICE9 pGraphic_Device) : CGameObject(pGraphic_Device) {};
	explicit CInteractable(const CInteractable& rhs) : CGameObject(rhs) {};
	virtual ~CInteractable() = default;

public:
	//pHitPos는 레이가 충돌한 지점의 위치다. (out 인자)
	virtual _bool		Picking(POINT _ViewPortPoint, _float3* pHitPos);
	virtual	void		Interact() = 0;
};
END
