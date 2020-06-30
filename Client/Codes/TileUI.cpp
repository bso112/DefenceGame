#include "stdafx.h"
#include "TileUI.h"
#include "Management.h"
#include "VIBuffer_ViewPort.h"
#include "KeyMgr.h"
#include "Clock.h"
USING(Client)

CTileUI::CTileUI(CTileUI & _rhs)
	: CUI(_rhs)
{

}


HRESULT CTileUI::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CTileUI::Ready_GameObject(void* pArg)
{
	if (nullptr != pArg)
		memcpy(&m_tDesc, pArg, sizeof(STATEDESC));


	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform)))
		return E_FAIL;
	if (FAILED(Add_Component(SCENE_STATIC, L"Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer)))
		return E_FAIL;
	//if (FAILED(Add_Component(m_tDesc.m_iTextureSceneID, m_tDesc.m_pTextureTag, L"Texture", (CComponent**)&m_pTexture)))
	//	return E_FAIL;
	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Shader_Rect_NoImg", L"Shader_Rect", (CComponent**)&m_pShader)))
		return E_FAIL;
	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;


	m_pTransform->SetUp_Position(m_tDesc.m_vPos);
	m_pTransform->Set_State(Engine::CTransform::STATE_LOOK, _float3(0.f, -1.f, 0.f));
	m_pTransform->Set_State(Engine::CTransform::STATE_UP, _float3(0.f, 0.f, 1.f));
	m_pTransform->Set_State(Engine::CTransform::STATE_RIGHT, _float3(1.f, 0.f, 0.f));
	m_bDead = 0;
	//m_pTransform->SetUp_Scale(m_tDesc.m_tBaseDesc.vSize);
	return S_OK;
}




_int CTileUI::Update_GameObject(_double _timeDelta)
{

	//if (m_bDead)
	//	return -1;

	//if (m_bExpand)
	//{
	//	if (nullptr == m_pTransform)
	//		return -1;

	//	_float3 vOldSize = m_pTransform->Get_Scaled();
	//	_float3 vSize = _float3(vOldSize.x + _float(m_vExpandSpeed.x * _timeDelta), vOldSize.y + _float(m_vExpandSpeed.y * _timeDelta), vOldSize.z + _float(m_vExpandSpeed.z * _timeDelta));
	//	m_pTransform->SetUp_Scale(vSize);
	//	if (vSize.x >= m_vMaxExpandSize.x || vSize.y >= m_vMaxExpandSize.y || vSize.z >= m_vMaxExpandSize.z)
	//		m_bExpand = false;
	//}


	return 0;
}


_int CTileUI::Late_Update_GameObject(_double _timeDelta)
{
	if (!m_bActive)
		return 0;

	if (nullptr == m_pRenderer)
		return -1;

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_3DUI, this)))
		return -1;

	return 0;
}

HRESULT CTileUI::Render_GameObject()
{
	//if (!m_bActive)
	//	return 0;

	if (nullptr == m_pVIBuffer ||
		nullptr == m_pTransform/* ||
		nullptr == m_pTexture*/)
		return E_FAIL;

	CManagement* pEngineMgr = CManagement::Get_Instance();

	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, false);

	_matrix matView = pEngineMgr->Get_Transform(D3DTS_VIEW);
	_matrix matProj = pEngineMgr->Get_Transform(D3DTS_PROJECTION);
	//if (FAILED(m_pVIBuffer->Set_Transform(m_pTransform->Get_WorldMatrix())))
	//	return E_FAIL;

	ALPHABLEND;

	//if (FAILED(m_pTexture->Set_TextureOnShader(m_pShader, "g_BaseTexture", 0)))
	//	return E_FAIL;

	if (FAILED(m_pShader->Set_Value("g_matWorld", &m_pTransform->Get_WorldMatrix(), sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Value("g_matView", &matView, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Value("g_matProj", &matProj, sizeof(_matrix))))
		return E_FAIL;

	if (FAILED(m_pShader->Begin_Shader()))
		return E_FAIL;
	if (FAILED(m_pShader->Begin_Pass(int(!m_tDesc.m_bIsOccupied))))
		return E_FAIL;


	if (FAILED(m_pVIBuffer->Render_VIBuffer()))
		return E_FAIL;


	if (FAILED(m_pShader->End_Pass()))
		return E_FAIL;

	if (FAILED(m_pShader->End_Shader()))
		return E_FAIL;


	ALPHABLEND_END;

	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, true);


	m_bDead = 1;

	return S_OK;
}




void CTileUI::Expand(_float3 _vMaxSize, _float3 _fExpandSpeed)
{
	m_bExpand = true;
	m_vMaxExpandSize = _vMaxSize;
	m_vExpandSpeed = _fExpandSpeed;
}



CTileUI * CTileUI::Create(PDIRECT3DDEVICE9 _pGraphic_Device)
{
	CTileUI* pInstance = new CTileUI(_pGraphic_Device);
	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Fail to create CTileUI");
		Safe_Release(pInstance);

	}
	return pInstance;
}

CGameObject * CTileUI::Clone_GameObject(void * pArg)
{
	CTileUI* pInstance = new CTileUI(*this);
	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Fail to clone CTileUI");
		Safe_Release(pInstance);

	}
	return pInstance;
}



void CTileUI::Free()
{

	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRenderer);

	CUI::Free();

}


_uint CTileUI::Get_Depth()
{
	//return m_tDesc.m_iDepth;
	return 0;
}


