#include "pch.h"
#include "mt.h"
#include "FtpClientDlg.h"
UINT mtQuery(LPVOID pParam)
{
	if (pParam == NULL)
		AfxEndThread(NULL);
	FTP_INFO* PP = (FTP_INFO*)pParam;
	CListBox* pList=PP->pList;
	CString strUrl=PP->strUrl;
	CString strUsername=PP->strUsername;
	CString strPwd=PP->strPwd;
	CInternetSession* pSession;
	CFtpConnection* pConnection=nullptr;
	CFtpFileFind* pFileFind=nullptr;
	CString strFileName;
	BOOL bContinue;
	pSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);
	try
	{
		pConnection = pSession->GetFtpConnection(strUrl, strUsername, strPwd, 21, TRUE);
	}
	catch (CInternetException * e)
	{
		e->Delete();
		pConnection = nullptr;
	}
	if (pConnection != nullptr)
	{
		pFileFind = new CFtpFileFind(pConnection);
		bContinue = pFileFind->FindFile();
		while (bContinue)
		{
			bContinue = pFileFind->FindNextFile();
			strFileName = pFileFind->GetFileName();
			if (pFileFind->IsDirectory())
				strFileName = TEXT("[") + strFileName + TEXT("]");
			pList->AddString(strFileName);
		}
		if (pFileFind != nullptr)
		{
			pFileFind->Close();
			pFileFind = nullptr;
		}
	}
	delete pFileFind;
	if (pConnection != nullptr)
	{
		pConnection->Close();
		delete pConnection;
	}
	delete pSession;
	return 0;
}

UINT mtDownloadFile(LPVOID pParam)
{
	if (pParam == NULL)
		AfxEndThread(NULL);
	FTP_INFO* PP = (FTP_INFO*)pParam;
	FTP_INFO* PP = (FTP_INFO*)pParam;
	CListBox* pList = PP->pList;
	CString strUrl = PP->strUrl;
	CString strUsername = PP->strUsername;
	CString strPwd = PP->strPwd;
	CInternetSession* pSession;
	CFtpConnection* pConnection = nullptr;
	int nSel = pList->GetCurSel();
	CString strSourceName;
	pList->GetText(nSel, strSourceName);
	if (strSourceName.GetAt(0) != '[')
	{
		CString strDestName = strSourceName;
		CFileDialog dlg(FALSE, TEXT(""), strDestName);
		if (dlg.DoModal() == IDOK)
		{
			strDestName = dlg.GetPathName();
			if (mtDownLoad(strUrl, strUsername, strPwd, strSourceName, strDestName))
				AfxMessageBox(TEXT("下载成功！"));
			else
			{
				AfxMessageBox(TEXT("下载失败！"));
				return 1;
			}
				
		}
		else
		{
			AfxMessageBox(TEXT("请输入文件名！"));
			return 2;
		}
	}
	else
	{
		AfxMessageBox(TEXT("不能下载目录！\n请重新选择"));
		return 3;
	}
	return 0;
}

BOOL mtDownLoad(CString strUrl, CString strUserName, CString strPwd, CString strSName, CString strDName)
{
	CInternetSession* pSession;
	CFtpConnection* pConnection = nullptr;
	pSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);
	try
	{
		pConnection = pSession->GetFtpConnection(strUrl, strUsername, strPwd, 21, TRUE);
	}
	catch (CInternetException * e)
	{
		e->Delete();
		pConnection = nullptr;
		return FALSE;
	}
	if (pConnection != NULL)
	{
		if (!pConnection->GetFile(strSName, strDName))
		{
			auto err = GetLastError();
			DWORD  dwError, dwBufferLength;
			LPTSTR  strError; // 将储存欢迎信息          
			// 获取欢迎信息   
			if (!InternetGetLastResponseInfo(&dwError, NULL, &dwBufferLength))
			{
				strError = new TCHAR[dwBufferLength + 1];
				InternetGetLastResponseInfo(&dwError, strError, &dwBufferLength);
				AfxMessageBox(strError);
				delete[] strError;
			}
			pConnection->Close();
			delete pConnection;
			delete pSession;
			return FALSE;
		}
	}
	pConnection->Close();
	delete pConnection;
	delete pSession;
	return TRUE;
}
