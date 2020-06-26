#include "stdafx.h"
#include "Cube.h"
#include "KeyMgr.h"
#include "GameManager.h"
#include "Camera_Free.h"
USING(Client)


CCube::CCube(PDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CCube::CCube(const CCube & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCube::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CCube::Ready_GameObject(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_tDesc, pArg, sizeof(STATEDESC));

	CTransform::STATEDESC tTransformDesc;
	tTransformDesc.RotatePerSec = D3DXToRadian(90.f);
	tTransformDesc.SpeedPerSec = 5.0;
	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform, &tTransformDesc)))
		return E_FAIL;

	if (FAILED(Add_Component(m_tDesc.eTextureSceneID, m_tDesc.pTextureTag, L"Com_Texture", (CComponent**)&m_pTexture)))
	return E_FAIL;
	

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Shader_Cube", L"Com_Shader_Cube", (CComponent**)&m_pShader)))
		return E_FAIL;

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_VIBuffer_Cube", L"Com_VIBuffer_Cube", (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_BoxCollider", L"Com_Collider", (CComponent**)&m_pBoxCollider)))
		return E_FAIL;

	m_pTransform->SetUp_Position(m_tDesc.tBaseDesc.vPos);

	m_pTransform->SetUp_Scale(m_tDesc.tBaseDesc.vSize);

	CKeyMgr::Get_Instance()->RegisterObserver(m_tDesc.eSceneID, this);
	return S_OK;
}

_int CCube::Update_GameObject(_double TimeDelta)
{
	

	if (nullptr == m_pBoxCollider	||
		nullptr == m_pTransform)
		return E_FAIL;

	m_pBoxCollider->Update_Collider(m_pTransform->Get_WorldMatrix());


	return _int();
}

_int CCube::Late_Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRenderer) return -1;
	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement) return -1;
	pManagement->Add_CollisionGroup(CCollisionMgr::COL_BOX, this);
	return 0;
}

HRESULT CCube::Render_GameObject()
{

	_matrix			matView, matProj;

	CManagement* pEnginMgr = CManagement::Get_Instance();
	if (nullptr == pEnginMgr) return E_FAIL;
	matView = pEnginMgr->Get_Transform(D3DTS_VIEW);
	matProj = pEnginMgr->Get_Transform(D3DTS_PROJECTION);

	if (FAILED(m_pTexture->Set_TextureOnShader(m_pShader, "g_DiffuseTexture", m_tDesc.iTextureID)))
		return E_FAIL;

	if (FAILED(m_pShader->Set_Value("g_matWorld", &m_pTransform->Get_WorldMatrix(), sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Value("g_matView", &matView, sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_Value("g_matProj", &matProj, sizeof(_matrix))))
		return E_FAIL;

	if (FAILED(m_pShader->Begin_Shader()))
		return E_FAIL;

	if (FAILED(m_pShader->Begin_Pass(m_tDesc.iShaderPass)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render_VIBuffer()))
		return E_FAIL;

	if (FAILED(m_pShader->End_Pass()))
		return E_FAIL;
	if (FAILED(m_pShader->End_Shader()))
		return E_FAIL;
	return S_OK;


}



CCube * CCube::Create(PDIRECT3DDEVICE9 pGraphic_Device)
{
	CCube*	pInstance = new CCube(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CBrick");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCube::Clone_GameObject(void * pArg)
{
	CCube*	pInstance = new CCube(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CBrick");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCube::Free()
{
	CKeyMgr::Get_Instance()->UnRegisterObserver(m_tDesc.eSceneID, this);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pTransform);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pBoxCollider);
	CGameObject::Free();
}
