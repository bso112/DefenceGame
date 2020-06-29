#include "..\Headers\VIBuffer_Brick.h"
#include "PipeLine.h"
#include "KeyMgr.h"

CVIBuffer_Brick::CVIBuffer_Brick(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Brick::CVIBuffer_Brick(const CVIBuffer_Brick & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Brick::Ready_Component_Prototype()
{
	//멤버변수 셋팅
	m_iStride = sizeof(VTXCUBETEX);
	m_iNumVertices = 8;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	m_iNumPrimitive = 12;
	m_iIndexSize = sizeof(_ushort);
	m_iNumIndices = m_iNumPrimitive * 3;
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	//값 채우기

	VTXCUBETEX* pVertices = nullptr;

	if (FAILED(m_pVBuffer->Lock(0, 0, (void**)&pVertices, 0)))
		return E_FAIL;

	/*
	큐브텍스쳐는 전개도처럼 되어있다. 
	왼쪽면, 오른쪽면, 윗면, 아랫면, 앞면, 뒷면 순서다.
	이때 UV는 원점 (0,0,0) 에서 각 정점으로 가는 방향벡터와 동일하다.
	pVertices[0].vTexUV = pVertices[0].vPosition - _float3(0,0,0);
	*/
	pVertices[0].vPosition = m_pVerticesPos[0] = _float3(-0.5f, 1.f, -0.5f);
	pVertices[0].vTexUV = pVertices[0].vPosition;
	pVertices[0].vTexUV.y -= 0.5f;

	pVertices[1].vPosition = m_pVerticesPos[1] = _float3(0.5f, 1.f, -0.5f);
	pVertices[1].vTexUV = pVertices[1].vPosition;
	pVertices[1].vTexUV.y -= 0.5f;

	pVertices[2].vPosition = m_pVerticesPos[2] = _float3(0.5f, 0.f, -0.5f);
	pVertices[2].vTexUV = pVertices[2].vPosition;
	pVertices[2].vTexUV.y -= 0.5f;

	pVertices[3].vPosition = m_pVerticesPos[3] = _float3(-0.5f, 0.f, -0.5f);
	pVertices[3].vTexUV = pVertices[3].vPosition;
	pVertices[3].vTexUV.y -= 0.5f;

	pVertices[4].vPosition = m_pVerticesPos[4] = _float3(-0.5f, 1.f, 0.5f);
	pVertices[4].vTexUV = pVertices[4].vPosition;
	pVertices[4].vTexUV.y -= 0.5f;

	pVertices[5].vPosition = m_pVerticesPos[5] = _float3(0.5f, 1.f, 0.5f);
	pVertices[5].vTexUV = pVertices[5].vPosition;
	pVertices[5].vTexUV.y -= 0.5f;

	pVertices[6].vPosition = m_pVerticesPos[6] = _float3(0.5f, 0.f, 0.5f);
	pVertices[6].vTexUV = pVertices[6].vPosition;
	pVertices[6].vTexUV.y -= 0.5f;

	pVertices[7].vPosition = m_pVerticesPos[7] = _float3(-0.5f, 0.f, 0.5f);
	pVertices[7].vTexUV = pVertices[7].vPosition;
	pVertices[7].vTexUV.y -= 0.5f;


	if (FAILED(m_pVBuffer->Unlock()))
		return E_FAIL;

	_ushort* pIndices = nullptr;

	if (FAILED(m_pIBuffer->Lock(0, 0, (void**)&pIndices, 0)))
		return E_FAIL;

	_uint		iIndex = 0;

	// +x
	pIndices[iIndex++] = 1; pIndices[iIndex++] = 5; pIndices[iIndex++] = 6;
	pIndices[iIndex++] = 1; pIndices[iIndex++] = 6; pIndices[iIndex++] = 2;

	// -x
	pIndices[iIndex++] = 4; pIndices[iIndex++] = 0; pIndices[iIndex++] = 3;
	pIndices[iIndex++] = 4; pIndices[iIndex++] = 3; pIndices[iIndex++] = 7;

	// y
	pIndices[iIndex++] = 4; pIndices[iIndex++] = 5; pIndices[iIndex++] = 1;
	pIndices[iIndex++] = 4; pIndices[iIndex++] = 1; pIndices[iIndex++] = 0;

	// -y
	pIndices[iIndex++] = 3; pIndices[iIndex++] = 2; pIndices[iIndex++] = 6;
	pIndices[iIndex++] = 3; pIndices[iIndex++] = 6; pIndices[iIndex++] = 7;

	// z
	pIndices[iIndex++] = 5; pIndices[iIndex++] = 4; pIndices[iIndex++] = 7;
	pIndices[iIndex++] = 5; pIndices[iIndex++] = 7; pIndices[iIndex++] = 6;

	// -z
	pIndices[iIndex++] = 0; pIndices[iIndex++] = 1; pIndices[iIndex++] = 2;
	pIndices[iIndex++] = 0; pIndices[iIndex++] = 2; pIndices[iIndex++] = 3;

	if (FAILED(m_pIBuffer->Unlock()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Brick::Ready_Component(void * pArg)
{
	return S_OK;
}

_bool CVIBuffer_Brick::Picking(POINT ptMouse, HWND hWnd, _matrix WorldMatrix, _float3 * pOut)
{
	if (!(GetKeyState(VK_LBUTTON) & 0x8000))
		return false;

	D3DVIEWPORT9		ViewPort;
	m_pGraphic_Device->GetViewport(&ViewPort);

	_float3		vMousePos = _float3((_float)ptMouse.x, (_float)ptMouse.y, 0.0f);

	// 2차원 투영 스페이스로 변환한다.(0, 0, g_iWinCX, g_iWinCY -> -1, 1, 1, -1)
	vMousePos.x = vMousePos.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = vMousePos.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영스페이스 -> 뷰스페이스 (투영행렬의 역행렬 곱하기)
	CPipeLine* pPipeLine = CPipeLine::Get_Instance();
	if (nullptr == pPipeLine)
		return false;
	Safe_AddRef(pPipeLine);

	_matrix InverseProjetion = pPipeLine->Get_Transform(D3DTS_PROJECTION);
	D3DXMatrixInverse(&InverseProjetion, nullptr, &InverseProjetion);

	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &InverseProjetion);

	// 뷰 스페이스에서의 마우스의 시작점, 레이를 구한다
	_float3		vMousePivot = _float3(0.f, 0.f, 0.f);
	_float3		vMouseRay = _float3(vMousePos.x, vMousePos.y, vMousePos.z) - vMousePivot;

	// 뷰스페이스 -> 월드스페이스 (뷰행렬의 역행렬을 구한다)
	_matrix ViewMatrix = pPipeLine->Get_Transform(D3DTS_VIEW);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	// 구한 뷰 공간의 역행렬을 마우스의 좌표에 곱한다. ( 마우스 좌표의 월드행렬화 )
	D3DXVec3TransformCoord(&vMousePivot, &vMousePivot, &ViewMatrix);
	D3DXVec3TransformNormal(&vMouseRay, &vMouseRay, &ViewMatrix);

	// 월드 -> 로컬 ( 월드스페이스의 역행렬을 구한다 )
	_matrix	WorldMatrixInverse;
	D3DXMatrixInverse(&WorldMatrixInverse, nullptr, &WorldMatrix);

	// 로컬스페이스 상의 마우스 레이, 마우스의 시작점을 구한다.
	D3DXVec3TransformCoord(&vMousePivot, &vMousePivot, &WorldMatrixInverse);
	D3DXVec3TransformNormal(&vMouseRay, &vMouseRay, &WorldMatrixInverse);

	// 마우스의 레이값을 노말라이즈
	D3DXVec3Normalize(&vMouseRay, &vMouseRay);

	VTXTEX* pVertices = nullptr;

	if (FAILED(m_pVBuffer->Lock(0, 0, (void**)&pVertices, 0)))
		return false;

	_float		fU, fV, fDist;

	if (D3DXIntersectTri(&pVertices[1].vPosition, &pVertices[0].vPosition, &pVertices[2].vPosition, &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
		goto except;
	if (D3DXIntersectTri(&pVertices[3].vPosition, &pVertices[0].vPosition, &pVertices[2].vPosition, &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
		goto except;

	if (D3DXIntersectTri(&pVertices[1].vPosition, &pVertices[5].vPosition, &pVertices[2].vPosition, &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
		goto except;
	if (D3DXIntersectTri(&pVertices[6].vPosition, &pVertices[2].vPosition, &pVertices[5].vPosition, &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
		goto except;
																																					   
	if (D3DXIntersectTri(&pVertices[5].vPosition, &pVertices[4].vPosition, &pVertices[6].vPosition, &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
		goto except;
	if (D3DXIntersectTri(&pVertices[7].vPosition, &pVertices[4].vPosition, &pVertices[6].vPosition, &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
		goto except;
																																					   
	if (D3DXIntersectTri(&pVertices[0].vPosition, &pVertices[4].vPosition, &pVertices[3].vPosition, &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
		goto except;
	if (D3DXIntersectTri(&pVertices[7].vPosition, &pVertices[3].vPosition, &pVertices[4].vPosition, &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
		goto except;
																																					   
	if (D3DXIntersectTri(&pVertices[0].vPosition, &pVertices[1].vPosition, &pVertices[4].vPosition, &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
		goto except;
	if (D3DXIntersectTri(&pVertices[5].vPosition, &pVertices[4].vPosition, &pVertices[1].vPosition, &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
		goto except;
																																					   
	if (D3DXIntersectTri(&pVertices[3].vPosition, &pVertices[2].vPosition, &pVertices[7].vPosition, &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
		goto except;
	if (D3DXIntersectTri(&pVertices[6].vPosition, &pVertices[2].vPosition, &pVertices[7].vPosition, &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
		goto except;

	if (FAILED(m_pVBuffer->Unlock()))
		return false;


	Safe_Release(pPipeLine);
	return false;

except:
	*pOut = vMousePivot + *D3DXVec3Normalize(&vMouseRay, &vMouseRay) * fDist;

	Safe_Release(pPipeLine);
	return true;
}



CVIBuffer_Brick * CVIBuffer_Brick::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Brick*	pInstance = new CVIBuffer_Brick(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		MessageBox(0, L"Failed To Creating CVIBuffer_Brick", L"System Message", MB_OK);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Brick::Clone_Component(void * pArg)
{
	CVIBuffer_Brick*	pInstance = new CVIBuffer_Brick(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MessageBox(0, L"Failed To Creating CVIBuffer_Rect", L"System Message", MB_OK);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Brick::Free()
{
	CVIBuffer::Free();
}


