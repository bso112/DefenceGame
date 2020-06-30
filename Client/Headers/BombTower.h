#pragma once
#include "Client_Defines.h"
#include "Building.h"
#include "Management.h"


BEGIN(Client)

class CBombTower final : public CBuilding
{
private:
	explicit CBombTower(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBombTower(const CBombTower& rhs);
	virtual ~CBombTower() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int Late_Update_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual void Upgrade();
	virtual void Interact();
public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollisionStay(CGameObject* _pOther);
private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
public:
	static CBombTower* Create(PDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END