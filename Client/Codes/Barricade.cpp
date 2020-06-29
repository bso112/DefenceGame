#include "stdafx.h"
#include "..\Headers\Barricade.h"
#include "Management.h"

CBarricade::CBarricade(PDIRECT3DDEVICE9 pGraphic_Device)
	: CBuilding(pGraphic_Device)
{

}

CBarricade::CBarricade(const CBarricade & rhs)
	: CBuilding(rhs)
{
	m_iTileSize = rhs.m_iTileSize;
	m_fScale = rhs.m_fScale;
}

HRESULT CBarricade::Ready_GameObject_Prototype()
{
	//건축물별 기본 필요정보 수정

	//필요 타일 크기
	m_iTileSize = 1;

	//건축물 자체 크기
	m_fScale = 1.f;

	return S_OK;
}

HRESULT CBarricade::Ready_GameObject(void * pArg)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tagStat.iMaxHP = CValue<int>(120);
	m_tagStat.iHp = m_tagStat.iMaxHP.GetValue();
	m_tagStat.iLevel = 1;
	m_tagStat.iPrice = 20;
	//m_pVIBufferCom->SetAnchor(_float3(0.f,-0.5f,0.f));

	m_fRealScaleMag = m_fScale * m_iTileSize;
	m_pTransformCom->SetUp_Scale(_float3(m_fRealScaleMag, m_fRealScaleMag, m_fRealScaleMag));
	m_pTransformCom->SetUp_Position(((CBuilding::BUILDING_DESC*)pArg)->vPos);

	return S_OK;
}

_int CBarricade::Update_GameObject(_double TimeDelta)
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	//m_pTransformCom->SetUp_Position(pManagement->Get_CamPosition());
	return _int();
}

_int CBarricade::Late_Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;


	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this)))
		return -1;

	return _int();
}

HRESULT CBarricade::Render_GameObject()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin_Shader();
	m_pShaderCom->Begin_Pass(RENDER_WARNING);

	m_pVIBufferCom->Render_VIBuffer();

	m_pShaderCom->End_Pass();
	m_pShaderCom->End_Shader();

	return S_OK;
}

void CBarricade::Upgrade()
{
}

void CBarricade::Interact()
{
}

HRESULT CBarricade::Add_Component()
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

HRESULT CBarricade::SetUp_ConstantTable()
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

CBarricade * CBarricade::Create(PDIRECT3DDEVICE9 pGraphic_Device)
{
	CBarricade*	pInstance = new CBarricade(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CBarricade");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBarricade::Clone_GameObject(void * pArg)
{
	CBarricade*	pInstance = new CBarricade(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CBarricade");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBarricade::Free()
{
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
