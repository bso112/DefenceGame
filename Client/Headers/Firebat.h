#pragma once
#include "Unit.h"
BEGIN(Client)
class CFirebat : public CUnit
{
public:
	typedef	struct tagStateDesc
	{
		BASEDESC			tBaseDesc;
		SCENEID				eTextureSceneID = SCENE_END;
		SCENEID				eSceneID = SCENE_END;
		const _tchar*		pTextureTag = nullptr;
		_uint				iShaderPass = 0;
		_uint				iTextureID = 0;

	}STATEDESC;
private:
	explicit CFirebat(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CFirebat(const CFirebat& _rhs);
	virtual ~CFirebat() = default;

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int Late_Update_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	//pHitPos�� ���̰� �浹�� ������ ��ġ��. (out ����)
	virtual _bool		Picking(POINT _ViewPortPoint, _float3* pHitPos);
	virtual	void		Interact();

public:
	virtual HRESULT	OnKeyDown(_int KeyCode);

private:
	CVIBuffer*		m_pVIBuffer = nullptr;
	CTexture*		m_pTexture = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CShader*		m_pShader = nullptr;
	CCollider_Box*	m_pBoxCollider = nullptr;
	STATEDESC		m_tDesc;

	//A*�� ���� Ÿ��
	CGameObject*	m_pTarget = nullptr;


public:
	virtual void OnCollisionEnter(CGameObject* _pOther) override;
	virtual void OnCollisionStay(CGameObject* _pOther);
public:
	static CFirebat*	Create(PDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void * pArg) override;
	virtual void Free();



};
END
