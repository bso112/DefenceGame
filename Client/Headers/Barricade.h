#pragma once
#include "Client_Defines.h"
#include "Building.h"
#include "Management.h"


BEGIN(Client)

class CBarricade final : public CBuilding
{
private:
	explicit CBarricade(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBarricade(const CBarricade& rhs);
	virtual ~CBarricade() = default;
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int Late_Update_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();

public:
	virtual void Upgrade();
	virtual void Interact();

private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
public:
	static CBarricade* Create(PDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
};

END