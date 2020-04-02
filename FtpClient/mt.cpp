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
				AfxMessageBox(TEXT("���سɹ���"));
			else
			{
				AfxMessageBox(TEXT("����ʧ�ܣ�"));
				return 1;
			}
				
		}
		else
		{
			AfxMessageBox(TEXT("�������ļ�����"));
			return 2;
		}
	}
	else
	{
		AfxMessageBox(TEXT("��������Ŀ¼��\n������ѡ��"));
		return 3;
	}
	return 0;
}

BOOL mtDownLoad(CString strUrl, CString strUserName, CString strPwd, CString strName, CString strDName)
{
	CInternetSession* pSession;
	CFtpConnection* pConnection = nullptr;
	pSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);
	try
	{
		pConnection = pSession->GetFtpConnection(strUrl, strName, strPwd, 21, TRUE);
	}
	catch (CInternetException * e)
	{
		e->Delete();
		pConnection = nullptr;
		return FALSE;
	}
	if (pConnection != NULL)
	{
		if (!pConnection->GetFile(strName, strDName))
		{
			auto err = GetLastError();
			DWORD  dwError, dwBufferLength;
			LPTSTR  strError; // �����滶ӭ��Ϣ          
			// ��ȡ��ӭ��Ϣ   
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

UINT mtUploadFile(LPVOID pParam)
{
	if (pParam == nullptr) AfxEndThread(NULL);
	FTP_INFO* PP;
	CListBox* pList;
	CString strUrl;
	CString strName;
	CString strPwd;
	PP = (FTP_INFO*)pParam;
	pList = PP->pList;
	strUrl = PP->strUrl;
	strName = PP->strUsername;
	strPwd = PP->strPwd;
	CString strSourceName;
	CString strDestName;
	CFileDialog dlg(TRUE, TEXT(""), TEXT("*.*"));
	if (dlg.DoModal() == IDOK)
	{
		strSourceName = dlg.GetPathName();
		strDestName = dlg.GetFileName();
		if (mtUpload(strUrl, strName, strPwd, strSourceName, strDestName))
			AfxMessageBox(TEXT("�ϴ��ɹ���"));
		else
		{
			AfxMessageBox(TEXT("�ϴ�ʧ�ܡ�"));
			return 1;
		}
	}
	else
	{
		AfxMessageBox(TEXT("��ѡ���ļ���"));
		return 1;
	}
	return 0;
}
BOOL mtUpload(CString strUrl, CString strName, CString strPwd, CString strSourcecName, CString strDestName)
{
	CInternetSession* pSession = nullptr;
	CFtpConnection* pConnection = nullptr;
	pSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);
	try
	{
		pConnection = pSession->GetFtpConnection(strUrl, strName, strPwd);
	}
	catch (CInternetException * e)
	{
		e->Delete();
		pConnection = nullptr;
		return 1;
	}
	if (pConnection != nullptr)
	{
		if (!pConnection->PutFile(strSourcecName, strDestName))
		{
			pConnection->Close();
			delete pConnection;
			delete pSession;
			return FALSE;
		}
	}
	if (pConnection != nullptr)
	{
		pConnection->Close();
		delete pConnection;
	}
	delete pSession;
	return TRUE;
};