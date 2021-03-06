#include "sledge/misc/binds.h"

#include <vector>
#include <mutex>

// from: WinUser.h
#define WM_KEYDOWN 0x0100
#define WM_KEYUP 0x0101

std::vector<CKeyBind*> vBinds;
std::mutex BindMutex;

void __RegisterBind(CKeyBind* Bind) {
	BindMutex.lock();
	vBinds.push_back(Bind);
	BindMutex.unlock();
}

void __UnregisterBind(CKeyBind* Bind) {
	BindMutex.lock();
	vBinds.erase(std::remove(vBinds.begin(), vBinds.end(), Bind), vBinds.end());
	BindMutex.unlock();
}

void Binds::OnInput(unsigned int uMsg, unsigned __int64 wParam) {
	for (CKeyBind* Bind : vBinds) {
		if (!Bind->m_Active)
			continue;

		if (Bind->m_BindType == EBindType::InputReader) {
			switch (uMsg) {
				case WM_KEYDOWN:
					reinterpret_cast<tBindInputReaderCallback>(Bind->m_Value)(static_cast<int>(wParam), true);
					break;
				case WM_KEYUP:
					reinterpret_cast<tBindInputReaderCallback>(Bind->m_Value)(static_cast<int>(wParam), false);
					break;
			}
			continue;
		}

		if (wParam == Bind->m_KeyId) {
			switch(uMsg) {
			case WM_KEYDOWN:
				Bind->OnKeyDown();
				break;
			case WM_KEYUP:
				Bind->OnKeyUp();
				break;
			}
		}
	}
}

CKeyBind::CKeyBind(EBindType eType, int iKeyCode, void* pValue, bool bActive) {
	this->m_BindType = eType;
	this->m_KeyId = iKeyCode;
	this->m_Active = bActive;
	this->m_Value = pValue;
	__RegisterBind(this);
}

CKeyBind::~CKeyBind() {
	__UnregisterBind(this);
}

void CKeyBind::OnKeyDown() {
	switch (this->m_BindType) {
		case EBindType::Callback: {
			reinterpret_cast<tBindFunction>(this->m_Value)();
			break;
		}

		case EBindType::Increase: {
			int* iValue = reinterpret_cast<int*>(this->m_Value);
			iValue++;
			break;
		}

		case EBindType::Decrease: {
			int* iValue = reinterpret_cast<int*>(this->m_Value);
			iValue--;
			break;
		}

		case EBindType::BoolToggle: {
			bool* bValue = reinterpret_cast<bool*>(this->m_Value);
			*bValue = !*bValue;
			break;
		}

		case EBindType::Bool: {
			bool* bValue = reinterpret_cast<bool*>(this->m_Value);
			*bValue = true;
			break;
		}
	}
}

void CKeyBind::OnKeyUp() {
	switch (this->m_BindType) {
		case EBindType::Bool: {
			bool* bValue = reinterpret_cast<bool*>(this->m_Value);
			*bValue = false;
			break;
		}
	}
}