#include "stdafx.h"
#include "..\Headers\CommandCenter.h"
#include "Management.h"
#include "PickingMgr.h"

CCommandCenter::CCommandCenter(PDIRECT3DDEVICE9 pGraphic_Device)
	: CBuilding(pGraphic_Device)
{
	m_tagStat.iPrice = 250;
}

CCommandCenter::CCommandCenter(const CCommandCenter & rhs)
	: CBuilding(rhs)
{
	m_iTileSize = rhs.m_iTileSize;
	m_fScale = rhs.m_fScale;
	m_tagStat.iPrice = rhs.m_tagStat.iPrice;
}

HRESULT CCommandCenter::Ready_GameObject_Prototype()
{
	//건축물별 기본 필요정보 수정

	//필요 타일 크기
	m_iTileSize = 3;

	//건축물 자체 크기
	m_fScale = 1.f;

	return S_OK;
}

HRESULT CCommandCenter::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fRealScaleMag = m_fScale * m_iTileSize;
	m_pTransformCom->SetUp_Scale(_float3(m_fRealScaleMag, m_fRealScaleMag, m_fRealScaleMag));
	m_pTransformCom->SetUp_Position(((CBuilding::BUILDING_DESC*)pArg)->vPos);

	CPickingMgr::Get_Instance()->Register_Observer(this);
	return S_OK;
}

_int CCommandCenter::Update_GameObject(_double TimeDelta)
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	m_tagStat.iMaxHP = CValue<int>(300);
	m_tagStat.iHp = m_tagStat.iMaxHP.GetValue();
	m_tagStat.iLevel = 1;

	return CBuilding::Update_GameObject(TimeDelta);
}

_int CCommandCenter::Late_Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;


	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this)))
		return -1;

	CBuilding::Late_Update_GameObject(TimeDelta);

	return _int();
}

HRESULT CCommandCenter::Render_GameObject()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin_Shader();
	m_pShaderCom->Begin_Pass(RENDER_RED);

	m_pVIBufferCom->Render_VIBuffer();

	m_pShaderCom->End_Pass();
	m_pShaderCom->End_Shader();

	return S_OK;
}

void CCommandCenter::Upgrade()
{
}

void CCommandCenter::Interact()
{
	CBuilding::Interact();

}

HRESULT CCommandCenter::Add_Component()
{
	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	//// For.Com_Texture
	//if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Cube", L"Com_Texture", (CComponent**)&m_pTextureCom)))
	//	return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_Brick", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Solid", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCommandCenter::SetUp_ConstantTable()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", &pManagement->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", &pManagement->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));


	//if (FAILED(m_pTextureCom->Set_TextureOnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
	//	return E_FAIL;



	Safe_Release(pManagement);

	return S_OK;
}

CCommandCenter * CCommandCenter::Create(PDIRECT3DDEVICE9 pGraphic_Device)
{
	CCommandCenter*	pInstance = new CCommandCenter(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CCommandCenter");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCommandCenter::Clone_GameObject(void * pArg)
{
	CCommandCenter*	pInstance = new CCommandCenter(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CCommandCenter");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCommandCenter::Free()
{
	CPickingMgr::Get_Instance()->UnRegister_Observer(this);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
