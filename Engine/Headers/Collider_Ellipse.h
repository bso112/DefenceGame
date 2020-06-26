#pragma once

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_Ellipse final : public CCollider
{
public:
	typedef struct tagStateDesc : public CCollider::STATEDESC
	{
		_float	fRadius;
		_float	fOffSetX, fOffSetY;
	}STATEDESC_DERIVED;
private:
	explicit CCollider_Ellipse(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCollider_Ellipse(const CCollider_Ellipse& rhs);
	virtual ~CCollider_Ellipse() = default;
public:
	virtual HRESULT Ready_Component_Prototype(const _tchar* _pTexturePath);
	virtual HRESULT Ready_Component(void* pArg);
	virtual _int Update_Collider(_matrix StateMatrix);
	virtual HRESULT Render_Collider();
public:
	_bool	Check_Collision(CCollider* pTarget);
private:
	LPD3DXSPRITE			m_pSprite = nullptr;	
	LPDIRECT3DTEXTURE9		m_pTexture = nullptr;
private:
	D3DSURFACE_DESC			m_SurfaceDesc;
	STATEDESC_DERIVED		m_StateDesc;
public:
	static CCollider_Ellipse* Create(PDIRECT3DDEVICE9 pGraphic_Device, const _tchar* _pTexturePat);
	virtual CComponent* Clone_Component(void* pArg);
	virtual void Free();
};

END