#pragma once
#include "GameObject.h"
#include "Component_Manager.h"
BEGIN(Client)
//class CTransform;
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
	virtual void Display_Stats() = 0;
	virtual _int Get_Cost() = 0;
	_bool Get_Friendly() { return m_bIsFriendly; };
	virtual CTransform* Get_Transform() = 0;

protected:
	_bool m_bIsFriendly = true;
};
END
