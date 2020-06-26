#include "stdafx.h"
#include "..\Headers\Wall.h"

USING(Client)

CWall::CWall(PDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{
}

CWall::CWall(const CWall & rhs)
	: CGameObject(rhs)
{
}

void CWall::OnCollisionEnter(CGameObject * _pOther)
{
	int a = 10;
}

void CWall::OnCollisionStay(CGameObject * _pOther)
{
	int a = 10;
}

HRESULT CWall::Ready_GameObject_Prototype()
{
	return S_OK;
}

HRESULT CWall::Ready_GameObject(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_tDesc, pArg, sizeof(STATEDESC));

	CTransform::STATEDESC tTransformDesc;
	tTransformDesc.RotatePerSec = D3DXToRadian(90.f);
	tTransformDesc.SpeedPerSec = 5.0;
	
	m_pName = m_tDesc.pName;
	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransform, &tTransformDesc)))
		return E_FAIL;

	if (FAILED(Add_Component(m_tDesc.eTextureSceneID, m_tDesc.pTextureTag, L"Texture", (CComponent**)&m_pTexture)))
	return E_FAIL;
	

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Shader_Rect", L"Com_Shader_Rect", (CComponent**)&m_pShader)))
		return E_FAIL;

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_VIBuffer_Rect", L"Com_VIBuffer_Rect", (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRenderer)))
		return E_FAIL;

	if (FAILED(Add_Component(SCENE_STATIC, L"Component_BoxCollider", L"Com_Collider", (CComponent**)&m_pBoxCollider)))
		return E_FAIL;

	m_pTransform->SetUp_Position(m_tDesc.tBaseDesc.vPos);

	m_pTransform->SetUp_Scale(m_tDesc.tBaseDesc.vSize);
	return S_OK;
}

_int CWall::Update_GameObject(_double TimeDelta)
{
	m_pBoxCollider->Update_Collider(m_pTransform->Get_WorldMatrix());

	return _int();
}

_int CWall::Late_Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;


	m_pRenderer->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

	CManagement* pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement) return -1;
	pManagement->Add_CollisionGroup(CCollisionMgr::COL_BOX, this);

	return _int();
}

HRESULT CWall::Render_GameObject()
{
	_matrix			matView, matProj;

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

	if (FAILED(m_pShader->Begin_Pass(PASS_WALL)))
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

CWall * CWall::Create(PDIRECT3DDEVICE9 pGraphic_Device)
{
	CWall*	pInstance = new CWall(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CField");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CWall::Clone_GameObject(void * pArg)
{
	CWall*	pInstance = new CWall(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CField");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWall::Free()
{
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
	Safe_Release(m_pTransform);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pBoxCollider);
	CGameObject::Free();
}
