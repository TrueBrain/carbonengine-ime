#include "StdAfx.h"

#if BLUE_WITH_PYTHON
#if IME_ENABLED

#include "Ime.h"

const char* BLUEMODULENAME = "_ime";
const char* g_moduleName = "Ime";

bool isWinNT = false;

static void StartDLL()
{
	CCP_LOG( "%s module starting", BLUEMODULENAME );
	BeClasses->RegisterClasses( BlueRegistration::GetClassRegs() );
}

#if BLUE_WITH_PYTHON
const char* InitializeForPython()
{
	const PyMethodDef dummyMethods[] = {0};

	// put myself into python as a module
	PyObject* module = Py_InitModule((char*)BLUEMODULENAME, (PyMethodDef*)dummyMethods);
	PyObject* dict = PyModule_GetDict(module);

	BlueRegisterToModule( module, BlueRegistration::GetClassRegs(), 
						  BlueRegistration::GetFuncRegs(),
						  BlueRegistration::GetEnumRegs(),
						  BlueRegistration::GetTestRegs(),
						  BlueRegistration::GetThunkerRegs());

	BlueRegisterObjectsToModule( module, BlueRegistration::GetObjectRegs() );
	BlueRegisterExceptionsToModule( module, BlueRegistration::GetExceptionRegs() );


	return NULL;
}
#endif


//--------------------------------------------------------------------
// initIme - python dll module entry function
//--------------------------------------------------------------------
extern "C" void
#ifdef _MSC_VER
	__declspec(dllexport)
#endif
init_ime ()
{
	StartDLL();
	InitializeForPython();
}

//#include "TriDirect3D.h" // For the def of WrappedHWND
#if BLUE_WITH_PYTHON
//typedefs for passing data opaquely around.
typedef BluePySWrap<Structify<HWND> > WrappedHWND;
#endif

#define MAX_COMPSTRING_SIZE 256
#define MAX_CANDLIST 10

HIMC Ime::s_hImcDef = NULL; // Default input context

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//
// Ime
//
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// Ime IPythonMethods Impl
/////////////////////////////////////////////////////////////////////////////////////////

void Ime::Destroy(
	)
{
	//delete this;
}


PyObject* Ime::GetAttr( 
	const char* name, 
	bool* handled
	)
{
	return 0;
}


bool Ime::SetAttr(
	const char* name, 
	PyObject* v, 
	bool* handled 
	)
{
	return true;
}


PyObject* Ime::Repr(
	bool* handled
	)
{
	*handled = true;
	return PyString_FromString("<Instance of 'Ime'>");
}

/////////////////////////////////////////////////////////////////////////////////////////
// Ime
/////////////////////////////////////////////////////////////////////////////////////////

Ime::Ime(IRoot* lockobj)
{
	m_hWnd = NULL;
	ImeWrapper::InitializeImm();
}

Ime::~Ime(void)
{
	ImeWrapper::UninitializeImm();
}

/////////////////////////////////////////////////////////////////////////////////////////
// Ime IIme Impl
/////////////////////////////////////////////////////////////////////////////////////////

//void Ime::SetupImeApi(){return;}

/////////////////////////////////////////////////////////////////////////////////////////
// Ime Python thunkers for IIme interface
/////////////////////////////////////////////////////////////////////////////////////////

PyObject* Ime::Py__init__(PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
		return 0;
	Py_INCREF(Py_None);
	return Py_None;
}

//SetHWND
PyObject* Ime::PySetHWND(PyObject* args)
{		
	PyObject *wHandle;

	if (!PyArg_ParseTuple(args, "O", &wHandle))
		return 0;

	if (PyInt_Check(wHandle))
	{
		unsigned long long handle;
		handle = PyInt_AsUnsignedLongLongMask(wHandle);
		m_hWnd = (HWND)handle;
	}
	else
	{
		WrappedHWND *hWnd_w = WrappedHWND::Unwrap(wHandle);

		if (!hWnd_w) {
			PyErr_SetString(PyExc_TypeError, "invalid 'handle'");
			return 0;
		}
			
		m_hWnd = *hWnd_w;
	}

	if (Ime::s_hImcDef == NULL)
	{
		Ime::s_hImcDef = ImeWrapper::ImmGetContext(m_hWnd);
		ImeWrapper::ImmReleaseContext(m_hWnd, Ime::s_hImcDef);
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* Ime::PyAssociateContext(PyObject* args)
{
	BOOL bShow;
	if (!PyArg_ParseTuple(args, "i", &bShow))
		return 0;

	// Setup the new IME that has been selected.
	ImeWrapper::ImmAssociateContext(m_hWnd, bShow ? Ime::s_hImcDef : NULL);

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* Ime::PyOnLanguageChanged(PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
		return 0;

	// Setup the new IME that has been selected.
	ImeWrapper::SetupImeApi();

	Py_INCREF(Py_None);
	return Py_None;
}

//ImmGetCompositionString
PyObject* Ime::PyGetCompositionString(PyObject* args)
{		
	DWORD dwIndex;

	if (!PyArg_ParseTuple(args, "i", &dwIndex))
		return 0;

	HIMC hIMC = CHIMC(this);

	DWORD dwIndexes[12] = {	GCS_CURSORPOS,
							GCS_RESULTSTR,
							GCS_COMPSTR,
							GCS_COMPATTR,
							GCS_COMPCLAUSE,
							
							GCS_COMPREADATTR,
							GCS_COMPREADCLAUSE,
							GCS_COMPREADSTR,
							GCS_DELTASTART,
							GCS_RESULTCLAUSE,
							GCS_RESULTREADCLAUSE,
							GCS_RESULTREADSTR
							};

	PyObject* dict = PyDict_New();

	for (int i = 0; i < (sizeof(dwIndexes)/sizeof(dwIndexes[0])); i++)
	{
		DWORD dwMask = dwIndexes[i];

		if ((dwIndex & dwMask) == 0)
			continue;

		switch(dwMask)
		{
			case GCS_CURSORPOS:
			case GCS_DELTASTART:
			{
				long lRes = (long)ImeWrapper::ImmGetCompositionStringW(hIMC, dwMask, NULL, 0);
				PyObject* value = PyInt_FromLong(lRes);
				PyObject* key = PyInt_FromLong(dwMask);
				PyDict_SetItem(dict, key, value);
				Py_DECREF(value);
				Py_DECREF(key);
				break;
			}

			default:
			{
				WCHAR wszCompStr[MAX_COMPSTRING_SIZE];
				ZeroMemory(wszCompStr, sizeof(WCHAR)*(MAX_COMPSTRING_SIZE));
				LONG lRes = ImeWrapper::ImmGetCompositionStringW(hIMC, dwMask, wszCompStr, sizeof(wszCompStr));

				if ((lRes == IMM_ERROR_NODATA)||(lRes == IMM_ERROR_GENERAL))
				{
					PyObject* value = PyInt_FromLong(lRes);
					PyObject* key = PyInt_FromLong(dwMask);
					PyDict_SetItem(dict, key, value);
					Py_DECREF(value);
					Py_DECREF(key);
				}
				else
				{
					PyObject* value = PyUnicode_FromWideChar((LPCWSTR)wszCompStr, lRes/2);
					PyObject* key = PyInt_FromLong(dwMask);
					PyDict_SetItem(dict, key, value);
					Py_DECREF(value);
					Py_DECREF(key);
				}

				/*LPBYTE lpBuf = NULL;
				LONG lRes = ImeWrapper::ImmGetCompositionStringW(hIMC, dwMask, lpBuf, 0);

				if ((lRes == IMM_ERROR_NODATA)||(lRes == IMM_ERROR_GENERAL))
				{
					PyObject* value = PyInt_FromLong(lRes);
					PyObject* key = PyInt_FromLong(dwMask);
					PyDict_SetItem(dict, key, value);
					Py_DECREF(value);
					Py_DECREF(key);
				}
				else
				{
					lpBuf = new BYTE[lRes+2];
					ZeroMemory(lpBuf, sizeof(BYTE)*(lRes+2));
					lRes = ImeWrapper::ImmGetCompositionStringW(hIMC, dwMask, lpBuf, lRes);

					PyObject* value = PyUnicode_FromWideChar((LPCWSTR)lpBuf, lRes/2);
					PyObject* key = PyInt_FromLong(dwMask);
					PyDict_SetItem(dict, key, value);
					Py_DECREF(value);
					Py_DECREF(key);

					delete [] lpBuf;
				}*/
				break;
			}
		}
	}

	return dict;
}
// NEED ImmReleaseContext
/*
PyObject* Ime::PyReleaseContext(PyObject* args)
{		
	if (!PyArg_ParseTuple(args, ""))
		return 0;
	ImmReleaseContext(m_hWnd, GetHIMC());
	m_hIMC = NULL;
	Py_INCREF(Py_None);
	return Py_None;
}
*/
//ImmGetCandidateList
PyObject* Ime::PyGetCandidateList(PyObject* args)
{		
	DWORD dwIndex = 0;

	if (!PyArg_ParseTuple(args, "|i", &dwIndex))
		return 0;

	HIMC hIMC = CHIMC(this);

    LPCANDIDATELIST lpCandList = NULL;
    DWORD dwLenRequired;

    // Retrieve the candidate list
	dwLenRequired = ImeWrapper::ImmGetCandidateListW(hIMC, dwIndex, NULL, 0);
    
	if (dwLenRequired)
    {
        lpCandList = (LPCANDIDATELIST)HeapAlloc(GetProcessHeap(), 0, dwLenRequired);
        dwLenRequired = ImeWrapper::ImmGetCandidateListW(hIMC, dwIndex, lpCandList, dwLenRequired);
    }

    if (lpCandList)
	{
		PyObject* dict = PyDict_New();

		PyObject* valueStyle = PyInt_FromLong(lpCandList->dwStyle);
		PyDict_SetItemString(dict, "Style", valueStyle);
		Py_DECREF(valueStyle);

		PyObject* valueSelection = PyInt_FromLong(lpCandList->dwSelection);
		PyDict_SetItemString(dict, "Selection", valueSelection);
		Py_DECREF(valueSelection);

		PyObject* valuePageStart = PyInt_FromLong(lpCandList->dwPageStart);
		PyDict_SetItemString(dict, "PageStart", valuePageStart);
		Py_DECREF(valuePageStart);

		PyObject* valuePageSize = PyInt_FromLong(lpCandList->dwPageSize);
		PyDict_SetItemString(dict, "PageSize", valuePageSize);
		Py_DECREF(valuePageSize);

		PyObject* list = PyList_New(lpCandList->dwCount);

		for (DWORD i = 0; i < lpCandList->dwCount; i++)
		{
			WCHAR *pwszNewCand = (LPWSTR)((LPBYTE)lpCandList + lpCandList->dwOffset[i]);
			
			PyList_SET_ITEM(list, i, PyUnicode_FromWideChar((LPCWSTR)pwszNewCand, lstrlenW(pwszNewCand)));
		}

		PyDict_SetItemString(dict, "Strings", list);
		Py_DECREF(list);

		HeapFree(GetProcessHeap(), 0, lpCandList);

		return dict;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

//ImmGetConversionStatus
PyObject* Ime::PyGetConversionStatus(PyObject* args)
{		
	if (!PyArg_ParseTuple(args, ""))
		return 0;

	HIMC hIMC = CHIMC(this);
	
	DWORD dwConversion = 0;
	DWORD dwSentence = 0;

	ImeWrapper::ImmGetConversionStatus(hIMC, &dwConversion, &dwSentence);
	
	PyObject* list = PyList_New(2);

	PyList_SET_ITEM(list, 0, PyInt_FromLong(dwConversion));
	PyList_SET_ITEM(list, 1, PyInt_FromLong(dwSentence));

	return list;
}

//ImmSetConversionStatus
PyObject* Ime::PySetConversionStatus(PyObject* args)
{		
	DWORD dwConversion = 0;
	DWORD dwSentence = 0;

	if (!PyArg_ParseTuple(args, "ii", &dwConversion, &dwSentence))
		return 0;

	HIMC hIMC = CHIMC(this);

	ImeWrapper::ImmSetConversionStatus(hIMC, dwConversion, dwSentence);

	Py_INCREF(Py_None);
	return Py_None;
}

//ImmGetConversionList
PyObject* Ime::PyGetConversionList(PyObject* args)
{		
	UINT uFlag = 0;
	PyUnicodeObject *srcString;

	if (!PyArg_ParseTuple(args, "Ui", &srcString, &uFlag))
		return 0;

	HIMC hIMC = CHIMC(this);

    LPCANDIDATELIST lpCandList = NULL;
    DWORD dwLenRequired;

    // Retrieve the candidate list
	HKL hKL = ImeWrapper::GetHKL();
	dwLenRequired = ImeWrapper::ImmGetConversionListW(hKL, hIMC, (LPCWSTR)PyUnicode_AS_DATA(srcString), NULL, 0, uFlag);
    
	if (dwLenRequired)
    {
        lpCandList = (LPCANDIDATELIST)HeapAlloc(GetProcessHeap(), 0, dwLenRequired);
        dwLenRequired = ImeWrapper::ImmGetConversionListW(hKL, hIMC, (LPCWSTR)PyUnicode_AS_DATA(srcString), lpCandList, dwLenRequired, uFlag);
    }

    if (lpCandList)
	{
		PyObject* dict = PyDict_New();

		PyObject* valueStyle = PyInt_FromLong(lpCandList->dwStyle);
		PyDict_SetItemString(dict, "Style", valueStyle);
		Py_DECREF(valueStyle);

		PyObject* valueSelection = PyInt_FromLong(lpCandList->dwSelection);
		PyDict_SetItemString(dict, "Selection", valueSelection);
		Py_DECREF(valueSelection);

		PyObject* valuePageStart = PyInt_FromLong(lpCandList->dwPageStart);
		PyDict_SetItemString(dict, "PageStart", valuePageStart);
		Py_DECREF(valuePageStart);

		PyObject* valuePageSize = PyInt_FromLong(lpCandList->dwPageSize);
		PyDict_SetItemString(dict, "PageSize", valuePageSize);
		Py_DECREF(valuePageSize);

		PyObject* list = PyList_New(lpCandList->dwCount);

		for (DWORD i = 0; i < lpCandList->dwCount; i++)
		{
			WCHAR *pwszNewCand = (LPWSTR)((LPBYTE)lpCandList + lpCandList->dwOffset[i]);
			
			PyList_SET_ITEM(list, i, PyUnicode_FromWideChar((LPCWSTR)pwszNewCand, lstrlenW(pwszNewCand)));
		}

		PyDict_SetItemString(dict, "Strings", list);
		Py_DECREF(list);

		HeapFree(GetProcessHeap(), 0, lpCandList);
		Py_DECREF(srcString);

		return dict;
	}

	Py_DECREF(srcString);
	Py_INCREF(Py_None);
	return Py_None;
}

//ImmIsIME
PyObject* Ime::PyIsIME(PyObject* args)
{
	unsigned long long lKl = (unsigned long long)ImeWrapper::GetHKL();
	if (!PyArg_ParseTuple(args, "|K", &lKl))
		return 0;

	return PyInt_FromLong(ImeWrapper::ImmIsIME((HKL)lKl));
}

PyObject* Ime::PyGetImeId(PyObject* args)
{
	UINT uIndex = 0;
	if (!PyArg_ParseTuple(args, "|i", &uIndex))
		return 0;

	return PyInt_FromLong(ImeWrapper::GetImeId(uIndex));
}

PyObject* Ime::PyGetReadingString(PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
		return 0;

	PyObject* dict = PyDict_New();
	
    do
	{
		bool s_bShowReadingWindow = false;
		bool s_bHorizontalReading = false;

		DWORD dwId = ImeWrapper::GetImeId();
		if (!dwId)
		{
			PyObject* valuePageSize = PyInt_FromLong(0);
			PyDict_SetItemString(dict, "bShowReadingWindow", valuePageSize);
			Py_DECREF(valuePageSize);
			break;
		}

		HIMC hIMC = CHIMC(this);
		if (!hIMC)
		{
			PyObject* valuePageSize = PyInt_FromLong(0);
			PyDict_SetItemString(dict, "bShowReadingWindow", valuePageSize);
			Py_DECREF(valuePageSize);
			break;
		}

		//WCHAR s_wszReadingString[32];
		DWORD dwReadingStrLen = 0;
		DWORD dwErr = 0;
		WCHAR *pwszReadingStringBuffer = NULL;  // Buffer for when the IME supports GetReadingString()
		WCHAR *wstr = 0;
		bool bUnicodeIme = false;  // Whether the IME context component is Unicode.
		INPUTCONTEXT *lpIC = NULL;

		if (ImeWrapper::HasGetReadingString())
		{
			UINT uMaxUiLen;
			BOOL bVertical;
			// Obtain the reading string size
			dwReadingStrLen = ImeWrapper::GetReadingString(hIMC, 0, NULL, (PINT)&dwErr, &bVertical, &uMaxUiLen);
			if (dwReadingStrLen)
			{
				wstr = pwszReadingStringBuffer = (LPWSTR)HeapAlloc(GetProcessHeap(), 0, sizeof(WCHAR) * dwReadingStrLen);
				if (!pwszReadingStringBuffer)
				{
					// Out of memory. Exit.
					break;
				}

				// Obtain the reading string
				dwReadingStrLen = ImeWrapper::GetReadingString(hIMC, dwReadingStrLen, wstr, (PINT)&dwErr, &bVertical, &uMaxUiLen);
			}

			s_bHorizontalReading = !bVertical;
			bUnicodeIme = true;
		}
		else
		{
			// IMEs that doesn't implement Reading String API
			lpIC = ImeWrapper::ImmLockIMC(hIMC);
	        
			LPBYTE p = 0;
			switch(dwId)
			{
				case IMEID_CHT_VER42: // New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
				case IMEID_CHT_VER43: // New(Phonetic/ChanJie)IME98a : 4.3.x.x // WinMe, Win2k
				case IMEID_CHT_VER44: // New ChanJie IME98b          : 4.4.x.x // WinXP
					p = *(LPBYTE *)((LPBYTE)ImeWrapper::ImmLockIMCC(lpIC->hPrivate) + 24);
					if (!p)
						break;
					dwReadingStrLen = *(DWORD *)(p + 7 * 4 + 32 * 4);
					dwErr = *(DWORD *)(p + 8 * 4 + 32 * 4);
					wstr = (WCHAR *)(p + 56);
					bUnicodeIme = true;
					break;

				case IMEID_CHT_VER50: // 5.0.x.x // WinME
					p = *(LPBYTE *)((LPBYTE)ImeWrapper::ImmLockIMCC(lpIC->hPrivate ) + 3 * 4);
					if (!p)
						break;
					p = *(LPBYTE *)((LPBYTE)p + 1*4 + 5*4 + 4*2);
					if (!p)
						break;
					dwReadingStrLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16);
					dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 + 1*4);
					wstr = (WCHAR *)(p + 1*4 + (16*2+2*4) + 5*4);
					bUnicodeIme = false;
					break;

				case IMEID_CHT_VER51: // 5.1.x.x // IME2002(w/OfficeXP)
				case IMEID_CHT_VER52: // 5.2.x.x // (w/whistler)
				case IMEID_CHS_VER53: // 5.3.x.x // SCIME2k or MSPY3 (w/OfficeXP and Whistler)
					p = *(LPBYTE *)((LPBYTE)ImeWrapper::ImmLockIMCC(lpIC->hPrivate ) + 4);
					if (!p)
						break;
					p = *(LPBYTE *)((LPBYTE)p + 1*4 + 5*4);
					if (!p)
						break;
					dwReadingStrLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2);
					dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2 + 1*4);
					wstr  = (WCHAR *) (p + 1*4 + (16*2+2*4) + 5*4);
					bUnicodeIme = true;
					break;

				// the code tested only with Win 98 SE (MSPY 1.5/ ver 4.1.0.21)
				case IMEID_CHS_VER41:
				{
					int nOffset;
					nOffset = (ImeWrapper::GetImeId(1) >= 0x00000002) ? 8 : 7;

					p = *(LPBYTE *)((LPBYTE)ImeWrapper::ImmLockIMCC(lpIC->hPrivate) + nOffset * 4);
					if (!p)
						break;
					dwReadingStrLen = *(DWORD *)(p + 7*4 + 16*2*4);
					dwErr = *(DWORD *)(p + 8*4 + 16*2*4);
					dwErr = std::min(dwErr, dwReadingStrLen);
					wstr = (WCHAR *)(p + 6*4 + 16*2*1);
					bUnicodeIme = true;
					break;
				}

				case IMEID_CHS_VER42: // 4.2.x.x // SCIME98 or MSPY2 (w/Office2k, Win2k, WinME, etc)
				{
					extern bool isWinNT;

					int nTcharSize = (isWinNT) ? sizeof(WCHAR) : sizeof(char);
					p = *(LPBYTE *)((LPBYTE)ImeWrapper::ImmLockIMCC(lpIC->hPrivate) + 1*4 + 1*4 + 6*4);
					if (!p)
						break;
					dwReadingStrLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize);
					dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize + 1*4);
					wstr  = (WCHAR *) (p + 1*4 + (16*2+2*4) + 5*4);
					bUnicodeIme = (isWinNT) ? true : false;
				}
			}   // switch
		}

		// Copy the reading string to the candidate list first
		//s_CandList.awszCandidate[0][0] = 0;
		//s_CandList.awszCandidate[1][0] = 0;
		//s_CandList.awszCandidate[2][0] = 0;
		//s_CandList.awszCandidate[3][0] = 0;
		//s_CandList.dwCount = dwReadingStrLen;
		//s_CandList.dwSelection = (DWORD)-1; // do not select any char
		
		DWORD dwCount = dwReadingStrLen;
		DWORD dwSelection = (DWORD)-1; // do not select any char
		DWORD dwPageSize = 0;
		int nReadingError = -1;

		PyObject* list = PyList_New(dwReadingStrLen);

		if (bUnicodeIme)
		{
			UINT i;
			for (i = 0; i < dwReadingStrLen; ++i) // dwlen > 0, if known IME
			{
				if (dwErr <= i && dwSelection == (DWORD)-1)
				{
					// select error char
					dwSelection = i;
				}

				PyList_SET_ITEM(list, i, PyInt_FromLong(wstr[i]));
			}
		}
		else
		{
			char *p = (char *)wstr;
			DWORD i, j, k;
			WCHAR w[1];
			for (i = 0, j = 0, k = 0; i < dwReadingStrLen; ++i, ++j) // dwlen > 0, if known IME
			{
				if (dwErr <= i && dwSelection == (DWORD)-1)
				{
					dwSelection = j;
				}
				// Obtain the current code page
				CHAR szCodePage[8];
				UINT uCodePage = CP_ACP;  // Default code page
				if( GetLocaleInfo( MAKELCID(ImeWrapper::GetLanguage(), SORT_DEFAULT ),
									LOCALE_IDEFAULTANSICODEPAGE,
									szCodePage,
									sizeof(szCodePage)/sizeof(szCodePage[0])))
				{
					uCodePage = strtoul(szCodePage, NULL, 0);
				}
				MultiByteToWideChar(uCodePage, 0, p + i, IsDBCSLeadByteEx(uCodePage, p[i]) ? 2 : 1, w, 1);
				PyList_SET_ITEM(list, k, PyInt_FromLong(w[0]));
				k++;
				if (IsDBCSLeadByteEx(uCodePage, p[i]))
					++i;
			}
			//s_CandList.awszCandidate[j][0] = 0;
			dwCount = j;
		}
		if (!ImeWrapper::HasGetReadingString())
		{
			ImeWrapper::ImmUnlockIMCC(lpIC->hPrivate);
			ImeWrapper::ImmUnlockIMC(hIMC);
			s_bHorizontalReading = ImeWrapper::GetReadingWindowOrientation(dwId);
		}
		//_ImmReleaseContext( DXUTGetHWND(), hImc );

		if (pwszReadingStringBuffer)
			HeapFree(GetProcessHeap(), 0, pwszReadingStringBuffer);

		// Copy the string to the reading string buffer
		if (dwCount > 0)
			s_bShowReadingWindow = true;
		else
			s_bShowReadingWindow = false;

		/*if (s_bHorizontalReading)
		{
			nReadingError = -1;
			s_wszReadingString[0] = 0;
			for (UINT i = 0; i < dwCount; ++i)
			{
				if (dwSelection == i)
					nReadingError = lstrlenW(s_wszReadingString);
				wcsncat(s_wszReadingString, s_CandList.awszCandidate[i], 32 - lstrlenW(s_wszReadingString) - 1);
			}
		}*/

		/*DWORD dwCount = dwReadingStrLen;
		DWORD dwSelection = (DWORD)-1; // do not select any char
		DWORD dwPageSize = 0;
		int nReadingError = -1;*/

		dwPageSize = MAX_CANDLIST;

		//bool s_bShowReadingWindow = false;
		//bool s_bHorizontalReading = false;

		PyObject* valueShowReadingWindow = PyInt_FromLong(s_bShowReadingWindow);
		PyDict_SetItemString(dict, "ShowReadingWindow", valueShowReadingWindow);
		Py_DECREF(valueShowReadingWindow);

		PyObject* valueHorizontalReading = PyInt_FromLong(s_bHorizontalReading);
		PyDict_SetItemString(dict, "HorizontalReading", valueHorizontalReading);
		Py_DECREF(valueHorizontalReading);

		PyObject* valueReadingError = PyInt_FromLong(nReadingError);
		PyDict_SetItemString(dict, "ReadingError", valueReadingError);
		Py_DECREF(valueReadingError);

		//PyObject* valueStyle = PyInt_FromLong(lpCandList->dwStyle);
		//PyDict_SetItemString(dict, "Style", valueStyle);
		//Py_DECREF(valueStyle);

		PyObject* valueSelection = PyInt_FromLong(dwSelection);
		PyDict_SetItemString(dict, "Selection", valueSelection);
		Py_DECREF(valueSelection);

		//PyObject* valuePageStart = PyInt_FromLong(lpCandList->dwPageStart);
		//PyDict_SetItemString(dict, "PageStart", valuePageStart);
		//Py_DECREF(valuePageStart);

		PyObject* valuePageSize = PyInt_FromLong(dwPageSize);
		PyDict_SetItemString(dict, "PageSize", valuePageSize);
		Py_DECREF(valuePageSize);

		PyDict_SetItemString(dict, "Strings", list);
		Py_DECREF(list);
	}
	while(false);

	return dict;
}

//ActivateKeyboardLayout HKL as an int or long
PyObject* Ime::PyActivateKeyboardLayout(PyObject* args)
{		
	unsigned long long lKL;
	UINT uFlags;
	if (!PyArg_ParseTuple(args, "Ki", &lKL, &uFlags))
		return 0;

	ActivateKeyboardLayout((HKL)lKL, uFlags);

	ImeWrapper::SetHKL((HKL)lKL);
	ImeWrapper::SetupImeApi();

	Py_INCREF(Py_None);
	return Py_None;
}

//HKL GetKeyboardLayout(::GetCurrentThreadId());
PyObject* Ime::PyGetKeyboardLayout(PyObject* args)
{		
	if (!PyArg_ParseTuple(args, ""))
		return 0;
	return PyInt_FromSize_t((size_t)GetKeyboardLayout(::GetCurrentThreadId()));
}

//GetKeyboardLayoutList
PyObject* Ime::PyGetKeyboardLayoutList(PyObject* args)
{		
	if (!PyArg_ParseTuple(args, ""))
		return 0;
	
	UINT uSize = GetKeyboardLayoutList(0, NULL);

	HKL *hKLs = new HKL[uSize];

	GetKeyboardLayoutList(uSize, hKLs);
	
	PyObject* list = PyList_New(uSize);

	for (UINT i = 0; i < uSize; i++)
	{
		PyList_SET_ITEM(list, i, PyInt_FromSize_t((size_t)hKLs[i]));
	}

	delete [] hKLs;

	return list;
}

PyObject* Ime::PyShowReadingWindow(PyObject* args)
{		
	BOOL bShow;
	if (!PyArg_ParseTuple(args, "i", &bShow))
		return 0;

	HIMC hIMC = CHIMC(this);

	ImeWrapper::ShowReadingWindow(hIMC, bShow);

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* Ime::PyNotifyIME(PyObject* args)
{		
	DWORD dwAction;
	DWORD dwIndex;
	DWORD dwValue = 0;

	if (!PyArg_ParseTuple(args, "ii|i", &dwAction, &dwIndex, &dwValue))
		return 0;

	HIMC hIMC = CHIMC(this);

	return PyInt_FromLong((long)ImeWrapper::ImmNotifyIME(hIMC, dwAction, dwIndex, dwValue));
}

PyObject* Ime::PyDisableTextFrameService(PyObject* args)
{
	DWORD dwThreadId = (DWORD)-1;
	if (!PyArg_ParseTuple(args, "|i", &dwThreadId))
		return 0;

	ImeWrapper::ImmDisableTextFrameService(dwThreadId);

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* Ime::PySimulateHotKey(PyObject* args)
{
	DWORD dwHotKeyID = 0;
	if (!PyArg_ParseTuple(args, "i", &dwHotKeyID))
		return 0;

	return PyInt_FromLong((long)ImeWrapper::ImmSimulateHotKey(m_hWnd, dwHotKeyID));
}

PyObject* Ime::PyGetOpenStatus(PyObject* args)
{
	if (!PyArg_ParseTuple(args, ""))
		return 0;

	HIMC hIMC = CHIMC(this);

	return PyInt_FromLong((long)ImeWrapper::ImmGetOpenStatus(hIMC));
}

PyObject* Ime::PySetOpenStatus(PyObject* args)
{		
	BOOL bShow;
	if (!PyArg_ParseTuple(args, "i", &bShow))
		return 0;

	HIMC hIMC = CHIMC(this);

	return PyInt_FromLong((long)ImeWrapper::ImmSetOpenStatus(hIMC, bShow));
}

// TODO: No support for GGL_STRING or GGL_PRIVATE
PyObject* Ime::PyGetGuideLine(PyObject* args)
{
	DWORD dwIndex;
	if (!PyArg_ParseTuple(args, "i", &dwIndex))
		return 0;

	HIMC hIMC = CHIMC(this);

	return PyInt_FromLong((long)ImeWrapper::ImmGetGuideLineW(hIMC, dwIndex, NULL, 0));
}

PyObject* Ime::PySetCompositionString(PyObject* args)
{
	DWORD dwIndex = 0;
	PyUnicodeObject *CompositionString;
	PyUnicodeObject *ReadingString;

	if (!PyArg_ParseTuple(args, "iUU", &dwIndex, &CompositionString, &ReadingString))
		return 0;

	//PyUnicode_AS_DATA(srcString)

	HIMC hIMC = CHIMC(this);

	const char *cdata = PyUnicode_AS_DATA(CompositionString);
	Py_ssize_t cdatalen = PyUnicode_GET_DATA_SIZE(CompositionString);
	const char *rdata = PyUnicode_AS_DATA(ReadingString);
	Py_ssize_t rdatalen = PyUnicode_GET_DATA_SIZE(ReadingString);
	BOOL bRes = ImeWrapper::ImmSetCompositionStringW(hIMC, dwIndex, (LPVOID)cdata, (DWORD)cdatalen, (LPVOID)rdata, (DWORD)rdatalen);
	return PyInt_FromLong((long)bRes);
}

PyObject* Ime::PyBackspace(PyObject* args)
{
	DWORD dwCount;
	if (!PyArg_ParseTuple(args, "i", &dwCount))
		return 0;

	while(dwCount>0x00000000)
	{
		dwCount--;

		keybd_event(VK_BACK, 0, 0, 0);
		keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0);
	}

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* Ime::PyGetIMEFileName(PyObject* args)
{
	unsigned long long lKL = (unsigned long long)::GetKeyboardLayout(::GetCurrentThreadId());
	if (!PyArg_ParseTuple(args, "|K", &lKL))
		return 0;

	CHAR szImeFile[MAX_PATH + 1];
	ZeroMemory(szImeFile, sizeof(CHAR)*(MAX_PATH+1));

	ImeWrapper::ImmGetIMEFileNameA((HKL)lKL, szImeFile, sizeof(szImeFile)/sizeof(szImeFile[0]) - 1);

	return PyString_FromString(szImeFile);
}

#endif
#endif
