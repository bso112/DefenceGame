#include "..\Headers\VIBuffer_Cube.h"
#include "PipeLine.h"
#include "KeyMgr.h"

CVIBuffer_Cube::CVIBuffer_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CVIBuffer(pGraphic_Device)
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Cube::Ready_Component_Prototype()
{
	//멤버변수 셋팅
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 8;
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);

	m_iNumPrimitive = 12;
	m_iIndexSize = sizeof(_ushort);
	m_iNumIndices = m_iNumPrimitive * 3;
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Component_Prototype()))
		return E_FAIL;

	//값 채우기

	VTXTEX* pVertices = nullptr;

	if (FAILED(m_pVBuffer->Lock(0, 0, (void**)&pVertices, 0)))
		return E_FAIL;

	pVertices[0].vPosition = m_pVerticesPos[0] = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vTexUV = _float2(0.f, 0.f);

	pVertices[1].vPosition = m_pVerticesPos[1] = _float3(0.5f, 0.5f, -0.5f);
	pVertices[1].vTexUV = _float2(1.f, 0.f);

	pVertices[2].vPosition = m_pVerticesPos[2] = _float3(0.5f, -0.5f, -0.5f);
	pVertices[2].vTexUV = _float2(1.f, 1.f);

	pVertices[3].vPosition = m_pVerticesPos[3] = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vTexUV = _float2(0.f, 1.f);

	pVertices[4].vPosition = m_pVerticesPos[0] = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vTexUV = _float2(0.f, 0.f);

	pVertices[5].vPosition = m_pVerticesPos[1] = _float3(0.5f, 0.5f, 0.5f);
	pVertices[5].vTexUV = _float2(1.f, 0.f);

	pVertices[6].vPosition = m_pVerticesPos[2] = _float3(0.5f, -0.5f, 0.5f);
	pVertices[6].vTexUV = _float2(1.f, 1.f);

	pVertices[7].vPosition = m_pVerticesPos[3] = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vTexUV = _float2(0.f, 1.f);


	if (FAILED(m_pVBuffer->Unlock()))
		return E_FAIL;

	_ushort* pIndices = nullptr;

	if (FAILED(m_pIBuffer->Lock(0, 0, (void**)&pIndices, 0)))
		return E_FAIL;

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;


	pIndices[6] = 4;
	pIndices[7] = 0;
	pIndices[8] = 3;

	pIndices[9] = 4;
	pIndices[10] = 3;
	pIndices[11] = 7;


	pIndices[12] = 1;
	pIndices[13] = 5;
	pIndices[14] = 6;

	pIndices[15] = 1;
	pIndices[16] = 6;
	pIndices[17] = 2;


	pIndices[18] = 4;
	pIndices[19] = 5;
	pIndices[20] = 1;

	pIndices[21] = 4;
	pIndices[22] = 1;
	pIndices[23] = 0;


	pIndices[24] = 6;
	pIndices[25] = 7;
	pIndices[26] = 3;

	pIndices[27] = 6;
	pIndices[28] = 3;
	pIndices[29] = 2;


	pIndices[30] = 5;
	pIndices[31] = 4;
	pIndices[32] = 7;

	pIndices[33] = 5;
	pIndices[34] = 7;
	pIndices[35] = 6;

	if (FAILED(m_pIBuffer->Unlock()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Cube::Ready_Component(void * pArg)
{
	return S_OK;
}

_bool CVIBuffer_Cube::Pick_Polygon(HWND hWnd, _matrix WorldMatrix, _float3 * pOut)
{
	if (!(GetKeyState(VK_LBUTTON) & 0x8000))
		return false;

	POINT		ptMouse;

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

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


	/*for (size_t i = 0; i < m_tStateDesc.iNumVerticesZ - 1; ++i)
	{
		for (size_t j = 0; j < m_tStateDesc.iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * m_tStateDesc.iNumVerticesX + j;

			_uint iIndices[4] = {
				{ iIndex + m_tStateDesc.iNumVerticesX },
				{ iIndex + m_tStateDesc.iNumVerticesX + 1 },
				{ iIndex + 1 },
				{ iIndex },
			};

			_float		fU, fV, fDist;

			// 우 상단 삼각형.
			if (true == D3DXIntersectTri(&m_pVerticesPos[iIndices[1]], &m_pVerticesPos[iIndices[0]], &m_pVerticesPos[iIndices[2]], &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
			{
				*pOut = m_pVerticesPos[iIndices[1]] + (m_pVerticesPos[iIndices[0]] - m_pVerticesPos[iIndices[1]]) * fU + (m_pVerticesPos[iIndices[2]] - m_pVerticesPos[iIndices[1]]) * fV;
				//*pOut = vMousePivot + *D3DXVec3Normalize(&vMouseRay, &vMouseRay) * fDist;
			}



			// 좌 하단 삼각형.
			if (true == D3DXIntersectTri(&m_pVerticesPos[iIndices[3]], &m_pVerticesPos[iIndices[2]], &m_pVerticesPos[iIndices[0]], &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
				int a = 10;
		}

	}*/

	Safe_Release(pPipeLine);
	return false;

except:
	*pOut = vMousePivot + *D3DXVec3Normalize(&vMouseRay, &vMouseRay) * fDist;

	Safe_Release(pPipeLine);
	return true;
}



CVIBuffer_Cube * CVIBuffer_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Cube*	pInstance = new CVIBuffer_Cube(pGraphic_Device);

	if (FAILED(pInstance->Ready_Component_Prototype()))
	{
		MessageBox(0, L"Failed To Creating CVIBuffer_Cube", L"System Message", MB_OK);
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Cube::Clone_Component(void * pArg)
{
	CVIBuffer_Cube*	pInstance = new CVIBuffer_Cube(*this);

	if (FAILED(pInstance->Ready_Component(pArg)))
	{
		MessageBox(0, L"Failed To Creating CVIBuffer_Rect", L"System Message", MB_OK);
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	CVIBuffer::Free();
}


