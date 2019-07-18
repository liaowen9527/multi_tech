#include "Clipboard.h"
#include "cstring_convert.h"

CClipboard::CClipboard(CWnd* pWnd)
	: m_pWnd(pWnd)
{

}

CClipboard::~CClipboard()
{

}

void CClipboard::Copy(const CString& strContent)
{
	CWaitCursor wait;

	if (!m_pWnd->OpenClipboard())
	{
#ifdef _DEBUG
		LPVOID lpMsgBuf;
		::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			::GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR)&lpMsgBuf,
			0,
			NULL
		);
		// Process any inserts in lpMsgBuf.
		// ...
		// Display the string.
		AfxMessageBox((LPCTSTR)lpMsgBuf, MB_OK | MB_ICONINFORMATION);

		// Free the buffer.
		::LocalFree(lpMsgBuf);
#endif
		return;
	}

	// prepare clipboard
	::EmptyClipboard();

	std::wstring wstr;
	CStringConverter::to_str(strContent, wstr);

	// Copy the buffer to clipboard
	BYTE *pBytes = (BYTE *)wstr.c_str();
	DWORD dwBytes = (DWORD)wstr.length() * 2;
	
	HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, dwBytes + sizeof(TCHAR));
	if (!hMem)
		return;
	LPVOID lpSource = (LPVOID)::GlobalLock(hMem);
	if (!lpSource)
		return;

	memcpy(lpSource, pBytes, dwBytes);
	::ZeroMemory(((BYTE *)lpSource + dwBytes), sizeof(TCHAR));

	UINT uCodePage = CP_ACP;

#ifdef _UNICODE
	// Determine the byte requirement
	int nLen = ::WideCharToMultiByte(uCodePage, 0, (LPWSTR)lpSource, -1, NULL, 0, NULL, NULL);

	HGLOBAL hMBCSMem = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, (nLen + 2));    // Maximum number of bytes is dwBytes
	if (!hMBCSMem)
	{
		CloseClipboard();
		return;
	}
	LPSTR lpMBCSSource = (LPSTR)::GlobalLock(hMBCSMem);
	if (!lpMBCSSource)
		return;

	int nBytes = ::WideCharToMultiByte(uCodePage, 0, (LPWSTR)lpSource, -1, lpMBCSSource, nLen, NULL, NULL);

	ASSERT(nBytes <= (int)dwBytes);

	lpMBCSSource[nBytes] = _T('\0');

	::GlobalUnlock(hMem);
	::GlobalUnlock(hMBCSMem);

	// lpSource is Unicode text
	::SetClipboardData(CF_UNICODETEXT, hMem);
	::SetClipboardData(CF_TEXT, hMBCSMem);
	::SetClipboardData(CF_OEMTEXT, hMBCSMem);
#else
	int nUBytes = (dwBytes + 2) * sizeof(WCHAR);
	HGLOBAL hUnicodeMem = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nUBytes);
	if (!hUnicodeMem)
	{
		CloseClipboard();
		return;
	}
	LPWSTR lpWSource = (LPWSTR)::GlobalLock(hUnicodeMem);
	if (!lpWSource)
	{
		CloseClipboard();
		return;
	}

	int nLen = ::MultiByteToWideChar(uCodePage, 0, (LPCSTR)lpSource, -1, lpWSource, nUBytes);

	ASSERT(nLen <= (int)(nUBytes / sizeof(WCHAR)));

	lpWSource[nLen] = _T('\0');

	::GlobalUnlock(hMem);
	::GlobalUnlock(hUnicodeMem);

	// lpSource is MBCS text
	::SetClipboardData(CF_TEXT, hMem);
	::SetClipboardData(CF_OEMTEXT, hMem);
	::SetClipboardData(CF_UNICODETEXT, hUnicodeMem);
#endif

	CloseClipboard();
}

void CClipboard::Paste(CString& strText)
{
	CWaitCursor wait;

	if (!m_pWnd->OpenClipboard())
		return;

#ifdef _UNICODE
	HANDLE hMem = ::GetClipboardData(CF_UNICODETEXT);
#else
	HANDLE hMem = ::GetClipboardData(CF_TEXT);
#endif

	if (!hMem)
	{
		return;
	}

	strText = (LPTSTR)::GlobalLock(hMem);

	::GlobalUnlock(hMem);

	CloseClipboard();
}
