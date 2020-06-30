#pragma once
#include "UI.h"
#include "Management.h"

BEGIN(Client)
class CClock_Single;
class CTileUI : public CUI
{
public:
	typedef struct tagStateDesc
	{
		_float3					m_vPos;
		_bool					m_bIsOccupied;


	}STATEDESC;

protected:
	explicit CTileUI(PDIRECT3DDEVICE9 _pGraphic_Device) :CUI(_pGraphic_Device) {};
	explicit CTileUI(CTileUI& _rhs);
	virtual ~CTileUI() = default;


protected:
	CVIBuffer*			m_pVIBuffer = nullptr;
	CTransform*			m_pTransform = nullptr;
	CTexture*			m_pTexture = nullptr;
	CShader*			m_pShader = nullptr;
	CRenderer*			m_pRenderer = nullptr;

protected:
	STATEDESC		m_tDesc;
	
public:
	_bool			m_bExpand = false;
	_float3			m_vMaxExpandSize;
	_float3			m_vExpandSpeed;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int Late_Update_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	void	Expand(_float3 _vMaxSize, _float3 _fExpandSpeed);
	virtual _uint Get_Depth();


public:
	static CTileUI*	Create(PDIRECT3DDEVICE9 _pGraphic_Device);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
public:

	virtual void Free() override;

};

END