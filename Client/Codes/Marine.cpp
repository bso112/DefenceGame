#include "stdafx.h"
#include "..\Headers\Marine.h"



CMarine::CMarine(PDIRECT3DDEVICE9 pGraphic_Device)
	:CUnit(pGraphic_Device)
{
}

CMarine::CMarine(const CMarine & _rhs)
	:CUnit(_rhs)
{
}

HRESULT CMarine::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CMarine::Ready_GameObject(void * pArg)
{
	return S_OK;
}

_int CMarine::Update_GameObject(_double TimeDelta)
{
	return _int();
}

_int CMarine::Late_Update_GameObject(_double TimeDelta)
{
	return _int();
}

HRESULT CMarine::Render_GameObject()
{
	return S_OK;
}

CMarine * CMarine::Create(PDIRECT3DDEVICE9 pGraphic_Device)
{
	CMarine*	pInstance = new CMarine(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CMarine");
		Safe_Release(pInstance);
	}
	return pInstance;
}


CGameObject * CMarine::Clone_GameObject(void * pArg)
{
	CMarine*	pInstance = new CMarine(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CMarine");
		Safe_Release(pInstance);
	}
	return pInstance;
}
void CMarine::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pTransform);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pBoxCollider);
	CGameObject::Free();
}
