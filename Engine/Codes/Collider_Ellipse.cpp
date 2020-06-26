#include "..\Headers\Collider_Ellipse.h"

USING(Engine)

CCollider_Ellipse::CCollider_Ellipse(PDIRECT3DDEVICE9 pGraphic_Device)
	: CCollider(pGraphic_Device)
{
}

CCollider_Ellipse::CCollider_Ellipse(const CCollider_Ellipse & rhs)
	: CCollider(rhs)
	, m_StateDesc(rhs.m_StateDesc)
	, m_pSprite(rhs.m_pSprite)
	, m_pTexture(rhs.m_pTexture)
	, m_SurfaceDesc(rhs.m_SurfaceDesc)
{
	Safe_AddRef(m_pTexture);
	Safe_AddRef(m_pSprite);
}

HRESULT CCollider_Ellipse::Ready_Component_Prototype(const _tchar* _pTexturePath)
{
	D3DXMatrixIdentity(&m_StateDesc.StateMatrix);

	m_StateDesc.fRadius = 0.5f;
	m_StateDesc.fOffSetX = 0.f;
	m_StateDesc.fOffSetY = 0.f;	

	if (FAILED(D3DXCreateSprite(m_pGraphic_Device, &m_pSprite)))
		return E_FAIL;

	if (FAILED(D3DXCreateTextureFromFile(m_pGraphic_Device, _pTexturePath, &m_pTexture)))
		return E_FAIL;

	m_pTexture->GetLevelDesc(0, &m_SurfaceDesc);

	return S_OK;
}

HRESULT CCollider_Ellipse::Ready_Component(void * pArg)
{
	// 복제할때 원의 사이즈와 오프셋을 받아온다. 위치도 받아오긴하지만, 업데이트에서 갱신될 정보.
	if (nullptr != pArg)
		memcpy(&m_StateDesc, pArg, sizeof(STATEDESC_DERIVED));

	return S_OK;
}

_int CCollider_Ellipse::Update_Collider(_matrix StateMatrix)
{
	D3DXVec3Normalize((_float3*)&m_StateDesc.StateMatrix.m[0][0], (_float3*)&StateMatrix.m[0][0]);
	D3DXVec3Normalize((_float3*)&m_StateDesc.StateMatrix.m[1][0], (_float3*)&StateMatrix.m[1][0]);
	D3DXVec3Normalize((_float3*)&m_StateDesc.StateMatrix.m[2][0], (_float3*)&StateMatrix.m[2][0]);

	memcpy(&m_StateDesc.StateMatrix.m[0][0], &(*(_float3*)&m_StateDesc.StateMatrix.m[0][0] * m_StateDesc.fRadius * 2.f), sizeof(_float3));
	memcpy(&m_StateDesc.StateMatrix.m[1][0], &(*(_float3*)&m_StateDesc.StateMatrix.m[1][0] * m_StateDesc.fRadius * 2.f), sizeof(_float3));

	StateMatrix.m[3][0] = StateMatrix.m[3][0] + m_StateDesc.fOffSetX;
	StateMatrix.m[3][1] = StateMatrix.m[3][1] + m_StateDesc.fOffSetY;

	memcpy(&m_StateDesc.StateMatrix.m[3][0], &StateMatrix.m[3][0], sizeof(_float3));

	return _int();
}

HRESULT CCollider_Ellipse::Render_Collider()
{
	if (nullptr == m_pSprite || 
		nullptr == m_pTexture || 
		nullptr == m_pPipeLine)
		return E_FAIL;


	m_pGraphic_Device->EndScene();

	m_pGraphic_Device->BeginScene();

	_matrix			RenderingMatrix;
	D3DXMatrixIdentity(&RenderingMatrix);

	D3DXMatrixScaling(&RenderingMatrix, (1.f / m_SurfaceDesc.Width) * m_StateDesc.fRadius * 2.f, (1.f / m_SurfaceDesc.Height) * m_StateDesc.fRadius * 2.f, 1.f);
	
	memcpy(&RenderingMatrix.m[3][0], &m_StateDesc.StateMatrix.m[3][0], sizeof(_float3));
	
	if (FAILED(m_pSprite->SetTransform(&(RenderingMatrix * m_pPipeLine->Get_Transform(D3DTS_VIEW)))))
		return E_FAIL;
	
	if (FAILED(m_pSprite->Begin(D3DXSPRITE_ALPHABLEND)))
		return E_FAIL;

	if (FAILED(m_pSprite->Draw(m_pTexture, nullptr, &_float3(_float(m_SurfaceDesc.Width >> 1), _float(m_SurfaceDesc.Height >> 1), 0.f), nullptr, 0xffffffff)))
		return E_FAIL;

	if (FAILED(m_pSprite->End()))
		return E_FAIL;


	m_pGraphic_Device->EndScene();

	m_pGraphic_Device->BeginScene();


	return S_OK;
}

_bool CCollider_Ellipse::Check_Collision(CCollider* pTarget)
{
	
	CCollider_Ellipse* pOther;
	pOther = dynamic_cast<CCollider_Ellipse*>(pTarget);

	if (nullptr == pOther)
	{
		MSG_BOX("허용된 콜라이더가 아닙니다.");
		return false;
	}

	_float3		vDirection = *(_float3*)&pOther->m_StateDesc.StateMatrix.m[3][0] - *(_float3*)&m_StateDesc.StateMatrix.m[3][0];

	_float		fDistance = D3DXVec3Length(&vDirection);

	m_isColl = true;
	pOther->m_isColl = true;

	if (m_StateDesc.fRadius + pOther->m_StateDesc.fRadius < fDistance)
	{
		m_isColl = false;
		pOther->m_isColl = false;
		return false;
	}

	return _bool(true);
}

CCollider_Ellipse * CCollider_Ellipse::Create(PDIRECT3DDEVICE9 pGraphic_Device, const _tchar* _pTexturePat)
{
	CCollider_Ellipse*	pInstance = new CCollider_Ellipse(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype(_pTexturePat)))
	{
		MessageBox(0, L"Failed To Creating CCollider_Ellipse", L"System Message", MB_OK);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider_Ellipse::Clone_Component(void * pArg)
{
	CCollider_Ellipse*	pInstance = new CCollider_Ellipse(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MessageBox(0, L"Failed To Creating CCollider_Ellipse", L"System Message", MB_OK);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider_Ellipse::Free()
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pSprite);

	CCollider::Free();
}
