#include "StdAfx.h"
#include "ImeWrapper.h"

#if IME_ENABLED

HINSTANCE ImeWrapper::s_hDllImm32;
LONG( WINAPI* ImeWrapper::m_fpImmGetCompositionStringW )( HIMC hIMC, DWORD dwIndex, LPVOID lpBuf, DWORD dwBufLen ) = nullptr;
HIMC( WINAPI* ImeWrapper::m_fpImmGetContext )( HWND hWnd ) = nullptr;
BOOL( WINAPI* ImeWrapper::m_fpImmReleaseContext )( HWND hWnd, HIMC hIMC ) = nullptr;
HIMC( WINAPI* ImeWrapper::m_fpImmAssociateContext )( HWND hWnd, HIMC hIMC ) = nullptr;
BOOL( WINAPI* ImeWrapper::m_fpImmGetOpenStatus )( HIMC hIMC ) = nullptr;
BOOL( WINAPI* ImeWrapper::m_fpImmSetOpenStatus )( HIMC hIMC, BOOL fOpen ) = nullptr;
UINT( WINAPI* ImeWrapper::m_fpImmGetVirtualKey )( HWND hWnd ) = nullptr;
BOOL( WINAPI* ImeWrapper::m_fpImmNotifyIME )( HIMC hIMC, DWORD dwAction, DWORD dwIndex, DWORD dwValue ) = nullptr;
BOOL( WINAPI* ImeWrapper::m_fpImmSimulateHotKey )( HWND hWnd, DWORD dwHotKeyID ) = nullptr;

void ImeWrapper::InitializeImm()
{
	wchar_t path[MAX_PATH + 1];
	if( !GetSystemDirectoryW( path, MAX_PATH + 1 ) )
	{
		return;
	}

	lstrcatW( path, L"\\imm32.dll" );
	s_hDllImm32 = LoadLibraryW( path );
	if( s_hDllImm32 )
	{
		*( (FARPROC*)&m_fpImmGetCompositionStringW ) = GetProcAddress( s_hDllImm32, "ImmGetCompositionStringW" );
		*( (FARPROC*)&m_fpImmGetContext ) = GetProcAddress( s_hDllImm32, "ImmGetContext" );
		*( (FARPROC*)&m_fpImmReleaseContext ) = GetProcAddress( s_hDllImm32, "ImmReleaseContext" );
		*( (FARPROC*)&m_fpImmAssociateContext ) = GetProcAddress( s_hDllImm32, "ImmAssociateContext" );
		*( (FARPROC*)&m_fpImmGetOpenStatus ) = GetProcAddress( s_hDllImm32, "ImmGetOpenStatus" );
		*( (FARPROC*)&m_fpImmSetOpenStatus ) = GetProcAddress( s_hDllImm32, "ImmSetOpenStatus" );
		*( (FARPROC*)&m_fpImmGetVirtualKey ) = GetProcAddress( s_hDllImm32, "ImmGetVirtualKey" );
		*( (FARPROC*)&m_fpImmNotifyIME ) = GetProcAddress( s_hDllImm32, "ImmNotifyIME" );
		*( (FARPROC*)&m_fpImmSimulateHotKey ) = GetProcAddress( s_hDllImm32, "ImmSimulateHotKey" );
	}
}


void ImeWrapper::UninitializeImm()
{
	if( s_hDllImm32 )
	{
		m_fpImmGetCompositionStringW = nullptr;
		m_fpImmGetContext = nullptr;
		m_fpImmReleaseContext = nullptr;
		m_fpImmAssociateContext = nullptr;
		m_fpImmGetOpenStatus = nullptr;
		m_fpImmSetOpenStatus = nullptr;
		m_fpImmGetVirtualKey = nullptr;
		m_fpImmNotifyIME = nullptr;
		m_fpImmSimulateHotKey = nullptr;

		FreeLibrary( s_hDllImm32 );
		s_hDllImm32 = nullptr;
	}
}

LONG ImeWrapper::ImmGetCompositionStringW( HIMC hIMC, DWORD dwIndex, LPVOID lpBuf, DWORD dwBufLen )
{
	return m_fpImmGetCompositionStringW ? m_fpImmGetCompositionStringW( hIMC, dwIndex, lpBuf, dwBufLen ) : IMM_ERROR_GENERAL;
}

HIMC ImeWrapper::ImmGetContext( HWND hWnd )
{
	return m_fpImmGetContext ? m_fpImmGetContext( hWnd ) : nullptr;
}

BOOL ImeWrapper::ImmReleaseContext( HWND hWnd, HIMC hIMC )
{
	return m_fpImmReleaseContext ? m_fpImmReleaseContext( hWnd, hIMC ) : FALSE;
}

HIMC ImeWrapper::ImmAssociateContext( HWND hWnd, HIMC hIMC )
{
	return m_fpImmAssociateContext ? m_fpImmAssociateContext( hWnd, hIMC ) : nullptr;
}

BOOL ImeWrapper::ImmGetOpenStatus( HIMC hIMC )
{
	return m_fpImmGetOpenStatus ? m_fpImmGetOpenStatus( hIMC ) : FALSE;
}

BOOL ImeWrapper::ImmSetOpenStatus( HIMC hIMC, BOOL fOpen )
{
	return m_fpImmSetOpenStatus ? m_fpImmSetOpenStatus( hIMC, fOpen ) : FALSE;
}

UINT ImeWrapper::ImmGetVirtualKey( HWND hWnd )
{
	return m_fpImmGetVirtualKey ? m_fpImmGetVirtualKey( hWnd ) : 0;
}

BOOL ImeWrapper::ImmNotifyIME( HIMC hIMC, DWORD dwAction, DWORD dwIndex, DWORD dwValue )
{
	return m_fpImmNotifyIME ? m_fpImmNotifyIME( hIMC, dwAction, dwIndex, dwValue ) : FALSE;
}

BOOL ImeWrapper::ImmSimulateHotKey( HWND hWnd, DWORD dwHotKeyID )
{
	return m_fpImmSimulateHotKey ? m_fpImmSimulateHotKey( hWnd, dwHotKeyID ) : FALSE;
}

#endif