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

	//���̽�Ÿ
	/*
	Gcost // distance from staring node
	Hcost // distance from end node
	Fcost // Gcost + Hcost
	*/

#pragma region �ʱ�ȭ

	for (int i = 0; i < TILEZ; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			if (nullptr != m_Nodes[i][j])
				m_Nodes[i][j]->Clear();
		}
	}

	//������ ���(������ΰ� �ƴϴ�)
	vector<NODE*> closed;
	//Ž���� ���
	set<NODE*> open;
	//�˻��� �߽��� �Ǵ� ������(�׳� visited�� ������ ���)
	NODE* pCurrNode = m_Nodes[srcZ][srcX];


	_int currX = srcX;
	_int currY = srcZ;

	_int minFcost = INT_MAX;
	//Fcost�� ������ Hcost �������� ��Ʈ����
	_int minHcost = INT_MAX;

	_int diaCost = (int)sqrt(TILESIZE * TILESIZE + TILESIZE * TILESIZE);

	int startHcost = 0;
	_int tmpX = abs(dstX - srcX);
	_int tmpY = abs(dstZ - srcZ);
	if (tmpX > tmpY)
		startHcost = tmpY * diaCost + (tmpX - tmpY) * TILESIZE;
	else
		startHcost = tmpX * diaCost + (tmpY - tmpX) * TILESIZE;

	//���۳�带 open����Ʈ�� ����
	pCurrNode->Set_Node(0, startHcost, startHcost, srcX, srcZ, nullptr);
	open.emplace(pCurrNode);

#pragma endregion

	//���̻� Ž���� ��尡 ����������
	while (open.size() > 0)
	{
		minFcost = INT_MAX;
		minHcost = INT_MAX;

		/*
		open set�� �˻��ؼ� Fcost�� ���� ���� ��带 ã�´�.
		����) ���� INT_MAX�� �ƴ� curFcost�� ���ϸ� Fcost�� ���� ��庸�� �� Ŀ���� ���� �����ع�����.
		��, ��ֹ��� ���ư��� ��δ� �ƿ� ������ ���ع���.
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

		//���õ� ���� open����Ʈ���� �����Ѵ�.
		open.erase(pCurrNode);
		//�湮�ߴٰ� ǥ��
		closed.push_back(pCurrNode);

		//�������� ���������� ��
		if (m_Nodes[dstZ][dstX] == pCurrNode)
			break;


		//�������� ���� �� Ÿ���� �ε���
		int tmpY = currY - 1;
		int tmpX = currX - 1;

		//�ֺ� 8 Ÿ���� �˻��ؼ� cost�� �����Ѵ�.
		for (int i = tmpY; i <= tmpY + 2; ++i)
		{
			for (int j = tmpX; j <= tmpX + 2; ++j)
			{
#pragma region ����ó��
				//��ȿ���� ���� Ÿ�� ����
				if (i >= TILEZ || j >= TILEX || i < 0 || j < 0)
					continue;
				if (nullptr == m_Nodes[i][j])
					continue;
				//���� ���ϴ� ���̸� ����
				if (!m_Nodes[i][j]->bMovable)
					continue;
				//�̹� �湮�� ���̸� ����
				if (find(closed.begin(), closed.end(), m_Nodes[i][j]) != closed.end())
					continue;
				//�ڱ��ڽ� ����
				if (i == currY && j == currX)
					continue;
				//�ڱ��ڽ��� nullptr�� ����
				if (nullptr == pCurrNode)
					return E_FAIL;

#pragma endregion
				//cost ���ϱ�
				_int Gcost = 0;
				_int Hcost = 0;
				_int Fcost = 0;

				//�� ������ �Ÿ�
				_int distX = abs(dstX - j);
				_int distY = abs(dstZ - i);

				/*
				�밢���� �̿��ؾ� �ִܰŸ���. ���� �밢���� �����.
				X�������� 1�� + Y�������� 1�� = �밢������ 1��
				*/
				//cost�� ����� ���;� �Ǳ� ������ ������ ó�� 
				if (distX > distY)
					Hcost = diaCost * distY + (distX - distY) * TILESIZE;
				else
					Hcost = diaCost * distX + (distY - distX) * TILESIZE;

				//��������� �Ÿ�
				distX = abs(currX - j);
				distY = abs(currY - i);

				if (distX > distY)
					Gcost = pCurrNode->Gcost + (diaCost * distY + (distX - distY) * TILESIZE);
				else
					Gcost = pCurrNode->Gcost + (diaCost * distX + (distY - distX) * TILESIZE);

				Fcost = Hcost + Gcost;

				//���� ���� Fcost�� ������ �ͺ��� ������ ���� ���� ����, �θ���
				if (m_Nodes[i][j]->Fcost > Fcost)
					m_Nodes[i][j]->Set_Node(Gcost, Hcost, Fcost, j, i, pCurrNode);

				//�˻��� �ִ´�. (�ߺ�����)
				open.emplace(m_Nodes[i][j]);


			}
		}
	}


	//visited�� �� �ִ°� �湮�ߴ� ��� ����. �� �߿��� ��θ� �̾Ƴ����Ѵ�.

	//��������
	NODE* pLastNode = closed.back();

	//���� ã�� ���������� ��ǥ�� ���� ����� ���� �������� ����
	if (open.size() == 0)
	{
		//����߿� Hcost�� ���� �������� ����������.
		//Hcost�� ������ Gcost�� �� ������ ����. (�� �̵��� �������� ����)
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

	//�θ� ����������(���۳�����)
	while (pLastNode->pParent != nullptr)
	{
		_out.push_back(pLastNode->vPos);
		//�θ� Ÿ�� ����
		pLastNode = pLastNode->pParent;
	}

	//�̾Ƴ��� ����� �ݴ��̱� ������ ����� ����
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
