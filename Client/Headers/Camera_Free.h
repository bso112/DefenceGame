#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Free final : public CCamera
{
private:
	explicit CCamera_Free(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera_Free(const CCamera_Free& rhs);
	virtual ~CCamera_Free() = default; 
public:
	_float GetCameraDistance(_float3 Pos);

public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int Late_Update_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();
public:
	static CCamera_Free* Create(PDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone_GameObject(void* pArg);
	virtual void Free();
private:
	_float3 m_vDirVec;

	_float3 CurMousePos;
	_float3 OldMousePos;
	_float3	fDir;

	_float	fCameraZommDist = 10.f;

	_float Angle = 0.f;

	_bool InitOnce = false;

};

END