#include "stdafx.h"
#include "..\Headers\Terrain.h"
#include "TileUI.h"
#include "Layer.h"

CTerrain::CTerrain(PDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	ZeroMemory(&m_Nodes, sizeof(m_Nodes));

}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{
	ZeroMemory(&m_Nodes, sizeof(m_Nodes));

}

HRESULT CTerrain::Ready_GameObject_Prototype()
{

	return S_OK;
}

HRESULT CTerrain::Ready_GameObject(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_tDesc, pArg, sizeof(STATEDESC));

	if (FAILED(Add_Component()))
		return E_FAIL;

	if (FAILED(Ready_Nodes()))
		return E_FAIL;

	CKeyMgr::Get_Instance()->RegisterObserver(m_tDesc._eSceneID, this);
	return S_OK;
}

_int CTerrain::Update_GameObject(_double TimeDelta)
{


	return _int();
}

_int CTerrain::Late_Update_GameObject(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	return m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
}

HRESULT CTerrain::Render_GameObject()
{
	if (nullptr == m_pVIBufferCom ||
		nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	m_pShaderCom->Begin_Shader();
	m_pShaderCom->Begin_Pass(0);

	m_pVIBufferCom->Render_VIBuffer();

	m_pShaderCom->End_Pass();
	m_pShaderCom->End_Shader();

	return S_OK;
}

HRESULT CTerrain::Get_Route(_float3 _src, _float3 _dst, vector<_float3>& _out)
{
	_out.clear();

	_int srcX = (_int)_src.x / TILESIZE;
	_int srcZ = (_int)_src.z / TILESIZE;
	_int dstX = (_int)_dst.x / TILESIZE;
	_int dstZ = (_int)_dst.z / TILESIZE;

	if (dstX >= TILEX || dstZ >= TILEZ)
		return E_FAIL;
	if (srcX >= TILEX || srcZ >= TILEZ)
		return E_FAIL;

	if (nullptr == m_Nodes[srcZ][srcX] || nullptr == m_Nodes[dstZ][dstX])
		return E_FAIL;

	//에이스타
	/*
	Gcost // distance from staring node
	Hcost // distance from end node
	Fcost // Gcost + Hcost
	*/

#pragma region 초기화

	for (int i = 0; i < TILEZ; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			if (nullptr != m_Nodes[i][j])
				m_Nodes[i][j]->Clear();
		}
	}

	//지나온 노드(최종경로가 아니다)
	vector<NODE*> closed;
	//탐사한 노드
	set<NODE*> open;
	//검사의 중심이 되는 현재노드(그냥 visited의 마지막 노드)
	NODE* pCurrNode = m_Nodes[srcZ][srcX];


	_int currX = srcX;
	_int currY = srcZ;

	_int minFcost = INT_MAX;
	//Fcost가 같으면 Hcost 기준으로 루트정함
	_int minHcost = INT_MAX;

	_int diaCost = (int)sqrt(TILESIZE * TILESIZE + TILESIZE * TILESIZE);

	int startHcost = 0;
	_int tmpX = abs(dstX - srcX);
	_int tmpY = abs(dstZ - srcZ);
	if (tmpX > tmpY)
		startHcost = tmpY * diaCost + (tmpX - tmpY) * TILESIZE;
	else
		startHcost = tmpX * diaCost + (tmpY - tmpX) * TILESIZE;

	//시작노드를 open리스트에 넣음
	pCurrNode->Set_Node(0, startHcost, startHcost, srcX, srcZ, nullptr);
	open.emplace(pCurrNode);

#pragma endregion

	//더이상 탐색할 노드가 없을때까지
	while (open.size() > 0)
	{
		minFcost = INT_MAX;
		minHcost = INT_MAX;

		/*
		open set을 검사해서 Fcost가 가장 작은 노드를 찾는다.
		주의) 만약 INT_MAX가 아닌 curFcost랑 비교하면 Fcost가 현재 노드보다 더 커지는 경우는 배제해버린다.
		즉, 장애물을 돌아가는 경로는 아예 생각도 안해버림.
		*/
		for (auto& node : open)
		{
			if (minFcost > node->Fcost)
			{
				minFcost = node->Fcost;
				pCurrNode = node;
				currX = node->X;
				currY = node->Z;
			}
			else if (minFcost == node->Fcost)
			{
				if (minHcost > node->Hcost)
				{
					minHcost = node->Hcost;
					pCurrNode = node;
					currX = node->X;
					currY = node->Z;
				}
			}

		}

		//선택된 노드는 open리스트에서 제거한다.
		open.erase(pCurrNode);
		//방문했다고 표시
		closed.push_back(pCurrNode);

		//목적지에 도달했으면 끝
		if (m_Nodes[dstZ][dstX] == pCurrNode)
			break;


		//현재노드의 왼쪽 위 타일의 인덱스
		int tmpY = currY - 1;
		int tmpX = currX - 1;

		//주변 8 타일을 검사해서 cost를 셋팅한다.
		for (int i = tmpY; i <= tmpY + 2; ++i)
		{
			for (int j = tmpX; j <= tmpX + 2; ++j)
			{
#pragma region 예외처리
				//유효하지 않은 타일 제외
				if (i >= TILEZ || j >= TILEX || i < 0 || j < 0)
					continue;
				if (nullptr == m_Nodes[i][j])
					continue;
				//가지 못하는 곳이면 제외
				if (!m_Nodes[i][j]->bMovable)
					continue;
				//이미 방문한 곳이면 제외
				if (find(closed.begin(), closed.end(), m_Nodes[i][j]) != closed.end())
					continue;
				//자기자신 제외
				if (i == currY && j == currX)
					continue;
				//자기자신이 nullptr면 끝냄
				if (nullptr == pCurrNode)
					return E_FAIL;

#pragma endregion
				//cost 구하기
				_int Gcost = 0;
				_int Hcost = 0;
				_int Fcost = 0;

				//끝 노드와의 거리
				_int distX = abs(dstX - j);
				_int distY = abs(dstZ - i);

				/*
				대각선을 이용해야 최단거리다. 따라서 대각선을 만든다.
				X방향으로 1번 + Y방향으로 1번 = 대각선으로 1번
				*/
				//cost는 양수가 나와야 되기 때문에 나눠서 처리 
				if (distX > distY)
					Hcost = diaCost * distY + (distX - distY) * TILESIZE;
				else
					Hcost = diaCost * distX + (distY - distX) * TILESIZE;

				//현재노드와의 거리
				distX = abs(currX - j);
				distY = abs(currY - i);

				if (distX > distY)
					Gcost = pCurrNode->Gcost + (diaCost * distY + (distX - distY) * TILESIZE);
				else
					Gcost = pCurrNode->Gcost + (diaCost * distX + (distY - distX) * TILESIZE);

				Fcost = Hcost + Gcost;

				//만약 구한 Fcost가 기존의 것보다 작으면 기존 것을 갱신, 부모설정
				if (m_Nodes[i][j]->Fcost > Fcost)
					m_Nodes[i][j]->Set_Node(Gcost, Hcost, Fcost, j, i, pCurrNode);

				//검사대상에 넣는다. (중복없이)
				open.emplace(m_Nodes[i][j]);


			}
		}
	}


	//visited에 들어가 있는건 방문했던 모든 노드다. 그 중에서 경로를 뽑아내야한다.

	//도착지점
	NODE* pLastNode = closed.back();

	//길을 찾지 못했을때는 목표와 가장 가까운 곳을 도착지로 설정
	if (open.size() == 0)
	{
		//경로중에 Hcost가 가장 낮은것을 목적지로함.
		//Hcost가 같으면 Gcost를 비교 낮은걸 취함. (더 이동이 적은것을 택함)
		if (closed.size() > 0)
		{
			pLastNode = closed[0];

			for (int i = 1; i < closed.size(); ++i)
			{
				_int lastHcost = pLastNode->Hcost;
				_int lastGcost = pLastNode->Gcost;
				_int currHcost = closed[i]->Hcost;
				_int currGcost = closed[i]->Gcost;

				if (lastHcost > currHcost)
				{
					pLastNode = closed[i];
				}
				else if (lastHcost == currHcost)
				{
					if (lastGcost > currGcost)
						pLastNode = closed[i];
				}
			}
		}
	}

	//부모가 없을때까지(시작노드까지)
	while (pLastNode->pParent != nullptr)
	{
		_out.push_back(pLastNode->vPos);
		//부모를 타고 들어가기
		pLastNode = pLastNode->pParent;
	}

	//뽑아낸건 경로의 반대이기 때문에 제대로 해줌
	reverse(_out.begin(), _out.end());
	return S_OK;
}

_bool CTerrain::Picking(POINT pt, _float3 * pOut)
{
	return m_pVIBufferCom->Picking(pt, g_hWnd, m_pTransformCom->Get_WorldMatrix(), pOut);
}

_bool CTerrain::BuildCheck(_float3* vPoint, _int ScaleInTiles)
{
	NODE tTempNode;

	CManagement* pManagement = CManagement::Get_Instance();

	_bool bIsFalse = 0;

	if (vPoint->x >= TILEX ||
		vPoint->z >= TILEZ ||
		vPoint->x < 0 ||
		vPoint->z < 0)
		return false;

	if (vPoint->x - ScaleInTiles + 1 < 0)
		vPoint->x = ScaleInTiles - 1;

	if (vPoint->z - ScaleInTiles + 1 < 0)
		vPoint->z = ScaleInTiles - 1;

	tTempNode.X = _uint(vPoint->x);
	tTempNode.Z = _uint(vPoint->z);

	CTileUI::STATEDESC tStateDesc;
	CLayer* tempLayer = pManagement->Find_Layer(SCENE_STATIC, L"Layer_TileUI");

	for (int i = tTempNode.Z - ScaleInTiles + 1; i <= tTempNode.Z; ++i)
	{
		for (int j = tTempNode.X - ScaleInTiles + 1; j <= tTempNode.X; ++j)
		{
			tStateDesc.m_bIsOccupied = 0;
			tStateDesc.m_vPos = _float3((_float)j + 0.5f, 0.f, (_float)i + 0.5f);

			_int iTileUICnt = 0;
			if (tempLayer != nullptr)
				iTileUICnt = tempLayer->Get_Size();

			if (m_Nodes[i][j]->bOccupied == 1)
			{
				tStateDesc.m_bIsOccupied = 1;
				if (iTileUICnt < ScaleInTiles*ScaleInTiles)
				{
					if (FAILED(pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_TileUI", SCENE_STATIC, L"Layer_TileUI", &tStateDesc)))
						return E_FAIL;

					bIsFalse = 1;
				}
			}
			if (iTileUICnt < ScaleInTiles*ScaleInTiles)
			{
				if (FAILED(pManagement->Add_Object_ToLayer(SCENE_STATIC, L"GameObject_TileUI", SCENE_STATIC, L"Layer_TileUI", &tStateDesc)))
					return E_FAIL;
			}
		}
	}

	if (ScaleInTiles % 2 == 0)
	{
		vPoint->x = _float(tTempNode.X - _uint(ScaleInTiles*0.5f) + 1);
		vPoint->z = _float(tTempNode.Z - _uint(ScaleInTiles*0.5f) + 1);
	}

	else
	{
		vPoint->x = _float(tTempNode.X - _uint(ScaleInTiles*0.5f)) + 0.5f;
		vPoint->z = _float(tTempNode.Z - _uint(ScaleInTiles*0.5f)) + 0.5f;
	}

	return !bIsFalse;
}

void CTerrain::Set_Occupation(_float3 * vPoint, _int ScaleInTiles, _bool bOccupation)
{
	int iTempX = int(vPoint->x);
	int iTempZ = int(vPoint->z);

	for (int i = iTempX - int(ScaleInTiles*0.5f); i <= iTempX + int(ScaleInTiles*0.5f) - _int(!ScaleInTiles) % 2; i++)
	{
		for (int j = iTempZ - int(ScaleInTiles*0.5f); j <= iTempZ + int(ScaleInTiles*0.5f) - _int(!ScaleInTiles) % 2; j++)
		{
			m_Nodes[j][i]->bOccupied = bOccupation;
		}
	}
	return;
}

HRESULT CTerrain::OnKeyDown(_int KeyCode)
{/*
 if (KeyCode == VK_LBUTTON)
 {
 _float3		vOut;
 if (m_pVIBufferCom->Picking(g_hWnd, m_pTransformCom->Get_WorldMatrix(), &vOut))
 {
 int a = 10;
 }
 }
 return S_OK;*/
	return S_OK;
}

HRESULT CTerrain::Add_Component()
{
	// For.Com_Renderer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Renderer", L"Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// For.Com_Transform
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Transform", L"Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// For.Com_VIBuffer
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_VIBuffer_Terrain", L"Com_VIBuffer", (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// For.Com_Shader
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Shader_Terrain", L"Com_Shader", (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// For.Com_Texture
	if (FAILED(CGameObject::Add_Component(SCENE_STATIC, L"Component_Texture_Terrain", L"Com_Texture", (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CTerrain::Ready_Nodes()
{
	for (int i = 0; i < TILEZ; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			m_Nodes[i][j] = new NODE(INT_MAX, INT_MAX, INT_MAX, j, i, nullptr);
			m_Nodes[i][j]->Set_Pos(_float3(j, 0, i));
			m_Nodes[i][j]->Set_Movable(true);

		}
	}
	return S_OK;
}

HRESULT CTerrain::SetUp_ConstantTable()
{
	CManagement*	pManagement = CManagement::Get_Instance();
	if (nullptr == pManagement)
		return E_FAIL;

	Safe_AddRef(pManagement);

	m_pShaderCom->Set_Value("g_matWorld", &m_pTransformCom->Get_WorldMatrix(), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matView", &pManagement->Get_Transform(D3DTS_VIEW), sizeof(_matrix));
	m_pShaderCom->Set_Value("g_matProj", &pManagement->Get_Transform(D3DTS_PROJECTION), sizeof(_matrix));

	m_pTextureCom->Set_TextureOnShader(m_pShaderCom, "g_DiffuseTexture");


	Safe_Release(pManagement);

	return S_OK;
}

CTerrain * CTerrain::Create(PDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain*	pInstance = new CTerrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
	{
		MSG_BOX("Failed To Create CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTerrain::Clone_GameObject(void * pArg)
{
	CTerrain*	pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Ready_GameObject(pArg)))
	{
		MSG_BOX("Failed To Create CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrain::Free()
{
	CKeyMgr::Get_Instance()->UnRegisterObserver(m_tDesc._eSceneID, this);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	for (int i = 0; i < TILEZ; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			delete m_Nodes[i][j];
		}
	}

	CGameObject::Free();
}
