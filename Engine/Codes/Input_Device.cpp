#include "..\Headers\Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
{

}

HRESULT CInput_Device::Ready_Input_Device(HINSTANCE g_hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(g_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pSDK, nullptr)))
		return E_FAIL;

	if (FAILED(m_pSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	//키보드라고 명확하게 명시해줌. c_dfDIKeyboard 은 전역변수다.
	if (FAILED(m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard)))
		return E_FAIL;
	/*
	협조레벨이란 hWnd가 다른 윈도우창과 협조하는 정도다. 
	예를들어 hWnd가 독점적으로 키보드 인풋을 받는다고 하면 다른 윈도우는 키보드 인풋을 받지 못한다.
	DISCL_NONEXCLUSIVE	: 다른 윈도우도 키보드 인풋을 받을 수 있다.
	DISCL_BACKGROUND	: 창이 내려가도 인풋이 가능하다. 창이 내려갔을때 인풋이 불가능하다고 하면 창을 내리고나면 키보드가 먹통이됨.
	주입력장치인 키보드나 마우스는 항상 DISCL_NONEXCLUSIVE | DISCL_BACKGROUND 이렇게 줘야한다.
	*/
	if (FAILED(m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND /*| DISCL_NOWINKEY*/)))
		return E_FAIL;
	//디바이스에 접근할 수 있는 제어권을 얻는다.
	if (FAILED(m_pKeyBoard->Acquire()))
		return E_FAIL;


	if (FAILED(m_pSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;
	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND /*| DISCL_NOWINKEY*/)))
		return E_FAIL;
	if (FAILED(m_pMouse->Acquire()))
		return E_FAIL;

	return S_OK;
}

HRESULT CInput_Device::Set_Input_Device_State()
{
	if (nullptr == m_pKeyBoard ||
		nullptr == m_pMouse)
		return E_FAIL;

	//키보드의 상태를 받는다.
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	//마우스의 상태를 받는다.
	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);

	return S_OK;
}

void CInput_Device::Free()
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pSDK);
}
