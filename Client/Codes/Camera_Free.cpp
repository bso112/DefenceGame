#include "stdafx.h"
#include "..\Headers\Camera_Free.h"
#include "GameManager.h"

CCamera_Free::CCamera_Free(PDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera(pGraphic_Device)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free & rhs)
	: CCamera(rhs)
{

}

_float CCamera_Free::GetCameraDistance(_float3 Pos)
{
	_float3 DirVec = Pos - m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	return D3DXVec3Length(&DirVec);
}

HRESULT CCamera_Free::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Free::Ready_GameObject(void * pArg)
{
	if (CCamera::Ready_GameObject(pArg))
		return E_FAIL;

	m_vDirVec = m_StateDesc.vEye - m_StateDesc.vAt;
	D3DXVec3Normalize(&m_vDirVec, &m_vDirVec);
	m_pTransformCom->SetUp_Position(_float3(0.f, 0.f, 0.f) + (m_vDirVec * fCameraZommDist));

	return S_OK;
}

_int CCamera_Free::Update_GameObject(_double TimeDelta)
{
	

	return CCamera::Update_GameObject(TimeDelta);
}

_int CCamera_Free::Late_Update_GameObject(_double TimeDelta)
{
	


	return CCamera::Late_Update_GameObject(TimeDelta);
}

HRESULT CCamera_Free::Render_GameObject()
{
	return S_OK;
}

CCamera_Free * CCamera_Free::Create(PDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera_Free*	pInstance = new CCamera_Free(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Creating CLoading");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCamera_Free::Clone_GameObject(void * pArg)
{
	CCamera_Free*	pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Creating CLoading");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Free::Free()
{
	CCamera::Free();
}
