#include "..\Headers\Transform.h"

CTransform::CTransform(PDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent(pGraphic_Device)
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{

}

_matrix CTransform::Get_WorldMatrix()
{
	return _matrix(m_WorldMatrix);
}

_matrix CTransform::Get_WorldMatrixInverse()
{
	_matrix		InverseMatrix;

	D3DXMatrixInverse(&InverseMatrix, nullptr, &m_WorldMatrix);

	return _matrix(InverseMatrix);
}

RECT CTransform::Get_Rect()
{
	//���� ��ġ�� Rect�� ���Ѵ�.
	RECT rc = {};
	float fX = Get_State(STATE_POSITION).x;
	float fY = Get_State(STATE_POSITION).y;
	_float3 vSize = Get_Scaled();
	int iCX = (int)vSize.x;
	int iCY = (int)vSize.y;

	rc.left = (LONG)fX - (iCX >> 1);
	rc.right = (LONG)fX + (iCX >> 1);
	rc.top = (LONG)fY - (iCY >> 1);
	rc.bottom = (LONG)fY + (iCY >> 1);
	return rc;
}

void CTransform::Set_State(STATE eState, _float3 vState)
{
	memcpy(&m_WorldMatrix.m[eState][0], &vState, sizeof(_float3));
}

HRESULT CTransform::Ready_Component_Prototype()
{
	D3DXMatrixIdentity(&m_WorldMatrix);	

	return S_OK;
}

HRESULT CTransform::Ready_Component(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_StateDesc, pArg, sizeof(STATEDESC));

	return S_OK;
}

HRESULT CTransform::Go_Straight(_double TimeDelta)
{
	_float3		vPosition, vLook;

	vLook = Get_State(CTransform::STATE_LOOK);
	vPosition = Get_State(CTransform::STATE_POSITION);

	// vPosition += ���⺤��1 * ���ǵ� * Ÿ�ӵ�Ÿ.;
	vPosition += *D3DXVec3Normalize(&vLook, &vLook) * _float(m_StateDesc.SpeedPerSec * TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_BackWard(_double TimeDelta)
{
	_float3		vPosition, vLook;

	vLook = Get_State(CTransform::STATE_LOOK);
	vPosition = Get_State(CTransform::STATE_POSITION);

	// vPosition += ���⺤��1 * ���ǵ� * Ÿ�ӵ�Ÿ.;
	vPosition -= *D3DXVec3Normalize(&vLook, &vLook) * _float(m_StateDesc.SpeedPerSec * TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Right(_double TimeDelta)
{
	_float3		vPosition, vRight;

	vRight = Get_State(CTransform::STATE_RIGHT);
	vPosition = Get_State(CTransform::STATE_POSITION);

	// vPosition += ���⺤��1 * ���ǵ� * Ÿ�ӵ�Ÿ.;
	vPosition += *D3DXVec3Normalize(&vRight, &vRight) * _float(m_StateDesc.SpeedPerSec * TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::Go_Left(_double TimeDelta)
{
	_float3		vPosition, vRight;

	vRight = Get_State(CTransform::STATE_RIGHT);
	vPosition = Get_State(CTransform::STATE_POSITION);

	// vPosition += ���⺤��1 * ���ǵ� * Ÿ�ӵ�Ÿ.;
	vPosition -= *D3DXVec3Normalize(&vRight, &vRight) * _float(m_StateDesc.SpeedPerSec * TimeDelta);

	Set_State(CTransform::STATE_POSITION, vPosition);

	return S_OK;
}

HRESULT CTransform::SetUp_Position(_float3 _vPos)
{

	Set_State(STATE_POSITION, _vPos);
	return S_OK;

}

HRESULT CTransform::SetUp_Scale(_float3 _vSize)
{
	_float3 vRight = *D3DXVec3Normalize(&Get_State(STATE_RIGHT), &Get_State(STATE_RIGHT)) * _vSize.x;
	_float3 vUp = *D3DXVec3Normalize(&Get_State(STATE_UP), &Get_State(STATE_UP)) * _vSize.y;
	_float3	vLook = *D3DXVec3Normalize(&Get_State(STATE_LOOK), &Get_State(STATE_LOOK)) * _vSize.z;
	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
	return S_OK;
}

// �׵���¿��� ���� ���ϴ� ȸ������ �ο��Ѵ�. != ���� ���¿��� ���� ���ϤѤ��� ȸ������ �߰��� �ο�����.
HRESULT CTransform::SetUp_Rotation(_float3 vAxis, _float fRadian)
{
	// ��Ŀ� ȸ���� ���¸� ��´�. == ����� 1, 2, 3�࿡ �ش��ϴ� ���⺤�͵��� ȸ�����ѳ��´�.
	_float3		vRight, vUp, vLook;

	_float3		vScale = Get_Scaled();

	vRight = _float3(1.f, 0.f, 0.f) * vScale.x;
	vUp = _float3(0.f, 1.f, 0.f) * vScale.y;
	vLook = _float3(0.f, 0.f, 1.f) * vScale.z;


	_matrix		RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Rotate(_float3 vAxis, _float fRadian)
{
	// ��Ŀ� ȸ���� ���¸� ��´�. == ����� 1, 2, 3�࿡ �ش��ϴ� ���⺤�͵��� ȸ�����ѳ��´�.
	_float3		vRight, vUp, vLook;


	vRight = Get_State(CTransform::STATE_RIGHT);
	vUp = Get_State(CTransform::STATE_UP);
	vLook = Get_State(CTransform::STATE_LOOK);

	_matrix		RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, fRadian);

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CTransform::Rotation_Axis(_float3 vAxis, _double TimeDelta)
{
	// ��Ŀ� ȸ���� ���¸� ��´�. == ����� 1, 2, 3�࿡ �ش��ϴ� ���⺤�͵��� ȸ�����ѳ��´�.
	_float3		vRight, vUp, vLook;

	vRight = Get_State(CTransform::STATE_RIGHT);
	vUp = Get_State(CTransform::STATE_UP);
	vLook = Get_State(CTransform::STATE_LOOK);

	_matrix		RotationMatrix;
	D3DXMatrixRotationAxis(&RotationMatrix, &vAxis, _float(m_StateDesc.RotatePerSec * TimeDelta));

	D3DXVec3TransformNormal(&vRight, &vRight, &RotationMatrix);
	D3DXVec3TransformNormal(&vUp, &vUp, &RotationMatrix);
	D3DXVec3TransformNormal(&vLook, &vLook, &RotationMatrix);

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

CTransform * CTransform::Create(PDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform*	pInstance = new CTransform(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		MSG_BOX("Failed To Create CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTransform::Clone_Component(void * pArg)
{
	CTransform*	pInstance = new CTransform(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MSG_BOX("Failed To Create CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTransform::Free()
{
	CComponent::Free();
}
