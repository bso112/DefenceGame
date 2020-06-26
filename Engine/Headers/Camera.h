#pragma once

#include "GameObject.h"
#include "Transform.h"
#include "PipeLine.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagStateDesc
	{
		// For.View
		_float3		vEye;
		_float3		vAt;
		_float3		vAxisY;

		// For.Projection
		_float		fFovy;
		_float		fAspect;
		_float		fNear;
		_float		fFar;

		// For.Transform
		CTransform::STATEDESC	TransformDesc;

	}STATEDESC;
protected:
	explicit CCamera(PDIRECT3DDEVICE9 pGraphic_Device);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;
public:
	void	Set_CameraFovy(_float _fRadian) { m_StateDesc.fFovy = _fRadian; }
	void	Set_DestCameraFovy(_float _fRadian) { m_fDestCameraFovy = _fRadian; }
public:
	virtual HRESULT Ready_GameObject_Prototype();
	virtual HRESULT Ready_GameObject(void* pArg);
	virtual _int Update_GameObject(_double TimeDelta);
	virtual _int Late_Update_GameObject(_double TimeDelta);
	virtual HRESULT Render_GameObject();
protected:
	CPipeLine*		m_pPipeLine;
	STATEDESC		m_StateDesc;
protected:
	CTransform*		m_pTransformCom = nullptr;
private:
	_float			m_fDestCameraFovy = 0.f;
private:
	void			CameraZoomInOutSmooth();
public:
	virtual CGameObject* Clone_GameObject(void* pArg) = 0;
	virtual void Free();
};

END