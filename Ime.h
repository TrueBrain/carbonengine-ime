/* 
	*************************************************************************************

	Ime.h

	Author:    Michael Brannan
	Created:   July 2005
	OS:        Win32
	Project:   Trinity

	Description:   

		Windows IME shim

	Dependencies:

		Windows

	(c) CCP 2005

	*************************************************************************************
*/


#ifndef _IME_H_
#define _IME_H_

#define IME_Description \
"na"

#include "include/IIme.h"
#include "ImeWrapper.h"

#if BLUE_WITH_PYTHON
class Ime :
	public IIme,
	public IPythonMethods
{
public:
	EXPOSE_TO_BLUE();

	Ime(IRoot* lockobj = NULL);
	~Ime(void);


	/////////////////////////////////////////////////////////////////////////////////////
	// IPythonMethods
	/////////////////////////////////////////////////////////////////////////////////////
	void Destroy(
		);

	PyObject* GetAttr( 
		const char* name, 
		bool* handled
		);

	bool SetAttr(
		const char* name,
		PyObject* v,
		bool* handled
		);

	PyObject* Repr(
		bool* handled
		);

public:
	PyObject* Py__init__( PyObject* args );
	PyObject* PySetHWND( PyObject* args );
	PyObject* PyAssociateContext( PyObject* args );
	PyObject* PyOnLanguageChanged( PyObject* args );
	PyObject* PyGetCompositionString( PyObject* args );
	PyObject* PyGetCandidateList( PyObject* args );
	PyObject* PyGetConversionStatus( PyObject* args );
	PyObject* PySetConversionStatus( PyObject* args );
	PyObject* PyGetConversionList( PyObject* args );
	PyObject* PyIsIME( PyObject* args );
	PyObject* PyGetImeId( PyObject* args );
	PyObject* PyGetReadingString( PyObject* args );
	PyObject* PyActivateKeyboardLayout( PyObject* args );
	PyObject* PyGetKeyboardLayout( PyObject* args );
	PyObject* PyGetKeyboardLayoutList( PyObject* args );
	PyObject* PyShowReadingWindow( PyObject* args );
	PyObject* PyNotifyIME( PyObject* args );
	PyObject* PyDisableTextFrameService( PyObject* args );
	PyObject* PySimulateHotKey( PyObject* args );
	PyObject* PyGetOpenStatus( PyObject* args );
	PyObject* PySetOpenStatus( PyObject* args );
	PyObject* PyGetGuideLine( PyObject* args );
	PyObject* PySetCompositionString( PyObject* args );
	PyObject* PyBackspace( PyObject* args );
	PyObject* PyGetIMEFileName( PyObject* args );

protected:
	HWND			m_hWnd;
	static HIMC		s_hImcDef;

	class CHIMC
	{
		Ime* m_pParent;
		HIMC	m_hIMC;
	public:
		CHIMC(Ime* pParent)
		{
			m_hIMC	  = NULL;
			m_pParent = pParent;
		}

		operator HIMC()
		{
			if (m_hIMC == NULL)
				m_hIMC = ImeWrapper::ImmGetContext(m_pParent->m_hWnd);
			return m_hIMC;
		}

		~CHIMC()
		{
			if (m_hIMC)
			{
				ImeWrapper::ImmReleaseContext(m_pParent->m_hWnd, m_hIMC);
				m_hIMC = NULL;
			}
		}

	};
};
TYPEDEF_BLUECLASS(Ime);

#endif
#endif // _TRIMIME_H_
