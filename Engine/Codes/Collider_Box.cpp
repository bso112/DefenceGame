#include "..\Headers\Collider_Box.h"

CCollider_Box::CCollider_Box(PDIRECT3DDEVICE9 pGraphic_Device)
	:CCollider(pGraphic_Device)
{
}

CCollider_Box::CCollider_Box(const CCollider_Box & rhs)
	: CCollider(rhs)
{
	memcpy(&m_vOriginalPoint, &rhs.m_vOriginalPoint, sizeof(_float3) * 8);
	memcpy(&m_vCurrPoint, &rhs.m_vCurrPoint, sizeof(_float3) * 8);
}

HRESULT CCollider_Box::Ready_Component_Prototype()
{
	m_vOriginalPoint[0] = _float3(-0.5f, 0.5f, -0.5f);
	m_vOriginalPoint[1] = _float3(0.5f, 0.5f, -0.5f);
	m_vOriginalPoint[2] = _float3(0.5f, -0.5f, -0.5f);
	m_vOriginalPoint[3] = _float3(-0.5f, -0.5f, -0.5f);
	m_vOriginalPoint[4] = _float3(-0.5f, 0.5f, 0.5f);
	m_vOriginalPoint[5] = _float3(0.5f, 0.5f, 0.5f);
	m_vOriginalPoint[6] = _float3(0.5f, -0.5f, 0.5f);
	m_vOriginalPoint[7] = _float3(-0.5f, -0.5f, 0.5f);

	memcpy(&m_vCurrPoint, &m_vOriginalPoint, sizeof(_float3) * 8);
	return S_OK;
}

HRESULT CCollider_Box::Ready_Component(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_tDesc, pArg, sizeof(STATEDESC_DERIVED));
	return S_OK;
}

_int CCollider_Box::Update_Collider(_matrix StateMatrix)
{
	m_tDesc.StateMatrix = StateMatrix;

	for (size_t i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vCurrPoint[i], &m_vOriginalPoint[i], &StateMatrix);
	}
	return 0;
}

HRESULT CCollider_Box::Render_Collider()
{
	return S_OK;
}

_bool CCollider_Box::Check_Collision(CCollider* pTarget)
{
	if (!m_bEnable)
		return false;
	CCollider_Box* pOther = dynamic_cast<CCollider_Box*>(pTarget);
	if (nullptr == pOther)
	{
		MSG_BOX("허용되는 콜라이더가 아닙니다.");
		return false;
	}

	_float3		vSourMin, vSourMax;
	_float3		vDestMin, vDestMax;

	vSourMin = m_vCurrPoint[3];
	vSourMax = m_vCurrPoint[5];

	vDestMin = pOther->m_vCurrPoint[3];
	vDestMax = pOther->m_vCurrPoint[5];

	_bool AxisX = false, AxisY = false, AxisZ = false;



	// x축선상에서 안겹쳤니?
	//더 작은 놈의 Max < 더 큰 놈의 Min 이면 false
	if (min(vSourMax.x, vDestMax.x) < max(vSourMin.x, vDestMin.x))
		return false;
	else
		AxisX = true;

	// y축선상에서 안겹쳤니?
	//더 작은놈의 Max < 더 큰놈의 Min 이면 false
	if (min(vSourMax.y, vDestMax.y) < max(vSourMin.y, vDestMin.y))
		return false;
	else
		AxisY = true;

	if (min(vSourMax.z, vDestMax.z) < max(vSourMin.z, vDestMin.z))
		return false;
	else
		AxisZ = true;

	// 충돌 길이 초기화
	m_vCollDistance = _float3(0.f, 0.f, 0.f);
	if (AxisX)
	{
		// Sour, Dest 의 X축 길이 
		_float SourLengthX = vSourMax.x - vSourMin.x;
		_float DestLengthX = vDestMax.x - vDestMin.x;

		// Sour, Dest의 X축 중점
		_float SourPivot = vSourMin.x + SourLengthX / 2.f;
		_float DestPivot = vDestMin.x + DestLengthX / 2.f;

		_float Distance = 0.f;

		// 중점간의 비교로 Sour의 우측일때 Sour의 좌측일때의 충돌 범위(길이)
		if (SourPivot < DestPivot)
			Distance = vSourMax.x - vDestMin.x;
		else if (SourPivot > DestPivot)
			Distance = vDestMax.x - vSourMin.x;

		m_vCollDistance.x = (Distance + 0.01f);
		pOther->m_vCollDistance.x = (Distance + 0.01f);
	}
	if (AxisY)
	{
		// Sour, Dest 의 X축 길이 
		_float SourLengthY = vSourMax.y - vSourMin.y;
		_float DestLengthY = vDestMax.y - vDestMin.y;

		// Sour, Dest의 X축 중점
		_float SourPivot = vSourMin.y + SourLengthY / 2.f;
		_float DestPivot = vDestMin.y + DestLengthY / 2.f;

		_float Distance = 0.f;

		// 중점간의 비교로 Sour의 우측일때 Sour의 좌측일때의 충돌 범위(길이)
		if (SourPivot < DestPivot)
			Distance = vSourMax.y - vDestMin.y;
		else if (SourPivot > DestPivot)
			Distance = vDestMax.y - vSourMin.y;

		m_vCollDistance.y = (Distance + 0.01f);
		pOther->m_vCollDistance.y = (Distance + 0.01f);
	}
	if (AxisZ)
	{
		// Sour, Dest 의 X축 길이 
		_float SourLengthZ = vSourMax.z - vSourMin.z;
		_float DestLengthZ = vDestMax.z - vDestMin.z;

		// Sour, Dest의 X축 중점
		_float SourPivot = vSourMin.z + SourLengthZ / 2.f;
		_float DestPivot = vDestMin.z + DestLengthZ / 2.f;

		_float Distance = 0.f;

		// 중점간의 비교로 Sour의 우측일때 Sour의 좌측일때의 충돌 범위(길이)
		if (SourPivot < DestPivot)
			Distance = vSourMax.z - vDestMin.z;
		else if (SourPivot > DestPivot)
			Distance = vDestMax.z - vSourMin.z;

		m_vCollDistance.z = (Distance + 0.01f);
		pOther->m_vCollDistance.z = (Distance + 0.01f);
	}

	return true;
}

CCollider_Box * CCollider_Box::Create(PDIRECT3DDEVICE9 pGraphic_Device)
{
	CCollider_Box*	pInstance = new CCollider_Box(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		MessageBox(0, L"Failed To Creating CCollider_Box", L"System Message", MB_OK);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider_Box::Clone_Component(void * pArg)
{
	CCollider_Box*	pInstance = new CCollider_Box(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MessageBox(0, L"Failed To Creating CCollider_Box", L"System Message", MB_OK);
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CCollider_Box::Free()
{
	CCollider::Free();
}
