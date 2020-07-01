#include "stdafx.h"
#include "Rim.h"

USING(Client)

CRim::CRim(PDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CRim::CRim(const CRim & rhs)
	: CGameObject(rhs)
{
}



HRESULT CRim::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CRim::Ready_GameObject(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_tDesc, pArg, sizeof(STATEDESC));

	CTransform::STATEDESC tTransformDesc;
	//tTransformDesc.RotatePerSec = D3DXToRadian(90.f);
	//tTransformDesc.SpeedPerSec = 5.0;

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform, &tTransformDesc)))
		return E_FAIL;

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Texture_Rim", L"Com_Texture", (CComponent**)&m_pTexture)))
		return E_FAIL;


	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Shader_Rect", L"Com_Shader_Rect", (CComponent**)&m_pShader)))
		return E_FAIL;

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;


	m_pTransform->SetUp_Position(m_tDesc.m_vPos + _float3(0.f,0.01f,0.f));

	m_pTransform->SetUp_Scale(m_tDesc.m_vScale*1.5f);

	m_pTransform->SetUp_Rotation(_float3(1.f,0.f,0.f),D3DXToRadian(270.f));

	
	return S_OK;
}

_int CRim::Update_GameObject(_double TimeDelta)
{

	return _int();
}

_int CRim::Late_Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;


	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

	return _int();
}

HRESULT CRim::Render_GameObject()
{
	_matrix			matView, matProj;
	m_bDead = 1;

	CManagement* pEnginMgr = CManagement::Get_Instance();
	if (nullptr == pEnginMgr) return E_FAIL;
	matView = pEnginMgr->Get_Transform(D3DTS_VIEW);
	matProj = pEnginMgr->Get_Transform(D3DTS_PROJECTION);

	if (FAILED(m_pTexture->Set_TextureOnShader(m_pShader, "g_BaseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pShader->Set_Value("g_matWorld", &m_pTransform->Get_WorldMatrix(), sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Value("g_matView", &matView, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Value("g_matProj", &matProj, sizeof(_matrix))))
		return E_FAIL;

	ALPHABLEND;

	if (FAILED(m_pShader->Begin_Shader()))
		return E_FAIL;

	if (FAILED(m_pShader->Begin_Pass(m_tDesc.m_bIsFriendly+3)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render_VIBuffer()))
		return E_FAIL;

	if (FAILED(m_pShader->End_Pass()))
		return E_FAIL;
	if (FAILED(m_pShader->End_Shader()))
		return E_FAIL;

	ALPHABLEND_END;
	return S_OK;
}

CRim * CRim::Create(PDIRECT3DDEVICE9 pGraphic_Device)
{
	CRim*	pInstance = new CRim(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CRim");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CRim::Clone_GameObject(void * pArg)
{
	CRim*	pInstance = new CRim(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CRim");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRim::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pTransform);
	Safe_Release(m_pVIBuffer);
	CGameObject::Free();
}
