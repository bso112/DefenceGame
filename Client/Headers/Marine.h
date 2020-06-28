#pragma once
#include "Unit.h"
#include "Management.h"
BEGIN(Client)
class CMarine : public CUnit
{
public:
	typedef	struct tagStateDesc
	{
		BASEDESC	tBaseDesc;
		UNITSTATS	tUnitStats;
	}STATEDESC;
private:
	explicit CMarine(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CMarine(const CMarine& _rhs);
	virtual ~CMarine() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int Late_Update_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

private:
	CTransform*		m_pTransform = nullptr;
	CVIBuffer*		m_pVIBuffer = nullptr;
	CTexture*		m_pTexture = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CShader*		m_pShader = nullptr;
	CCollider_Box*	m_pBoxCollider = nullptr;

public:
	static CMarine*	Create(PDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void * pArg) override;
	virtual void Free();

};
END
