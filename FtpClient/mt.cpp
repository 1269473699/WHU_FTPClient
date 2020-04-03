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
	CString strCurDir = PP->strCurrentDir;	
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
		if (!strCurDir.IsEmpty() && strCurDir.GetLength() != 0)
		{
			pConnection->SetCurrentDirectory(strCurDir);
		}
		CString strCurrentDir;
		pConnection->GetCurrentDirectory(strCurrentDir);
		AfxGetMainWnd()->SendMessage(WM_DIR_CHANGE, (WPARAM)&strCurrentDir);
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
	CListBox* pList = PP->pList;
	CString strUrl = PP->strUrl;
	CString strUsername = PP->strUsername;
	CString strPwd = PP->strPwd;
	CString strCurDir = PP->strCurrentDir;
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
	}
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
			if (mtDownLoad(strUrl, strUsername, strPwd, strSourceName, strDestName, strCurDir))
				AfxMessageBox(TEXT("下载成功！"));
			else
			{
				AfxMessageBox(TEXT("下载失败！"));
				AfxGetMainWnd()->SendMessage(WM_DOWNLOAD_FIN, 0, 0);
				return 1;
			}
				
		}
		else
		{
			AfxMessageBox(TEXT("请输入文件名！"));
			AfxGetMainWnd()->SendMessage(WM_DOWNLOAD_FIN, 0, 0);
			return 2;
		}
	}
	else
	{
		AfxMessageBox(TEXT("不能下载目录！\n请重新选择"));
		AfxGetMainWnd()->SendMessage(WM_DOWNLOAD_FIN, 0, 0);
		return 3;
	}
	AfxGetMainWnd()->SendMessage(WM_DOWNLOAD_FIN, 0, 0);
	return 0;
}

BOOL mtDownLoad(CString strUrl, CString strUserName, CString strPwd, CString strName, CString strDName, CString strCurDir)
{
	CInternetSession* pSession;
	CFtpConnection* pConnection = nullptr;
	pSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);
	try
	{
		pConnection = pSession->GetFtpConnection(strUrl, strUserName, strPwd, 21, TRUE);
	}
	catch (CInternetException * e)
	{
		e->Delete();
		pConnection = nullptr;
		return FALSE;
	}
	if (pConnection != NULL)
	{
		pConnection->SetCurrentDirectory(strCurDir);
		CFtpFileFind* pFtpFFind = new CFtpFileFind(pConnection);
		BOOL FtpFile = pFtpFFind->FindFile(strName);//在FTP上找到文件
		FtpFile = pFtpFFind->FindNextFile();
		int nFileSize = pFtpFFind->GetLength();
		FILE_INFO* ptF = new FILE_INFO;
		ptF->nFileSize = nFileSize;
		ptF->strDName = strDName;
		AfxGetMainWnd()->PostMessage(WM_DOWNLOAD_ST, (WPARAM)ptF);
		if (!pConnection->GetFile(strName, strDName))
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
	CString strCurDir = PP->strCurrentDir;
	if (dlg.DoModal() == IDOK)
	{
		strSourceName = dlg.GetPathName();
		strDestName = dlg.GetFileName();
		if (mtUpload(strUrl, strName, strPwd, strSourceName, strDestName, strCurDir))
			AfxMessageBox(TEXT("上传成功。"));
		else
		{
			AfxMessageBox(TEXT("上传失败。"));
			return 1;
		}
	}
	else
	{
		AfxMessageBox(TEXT("请选择文件。"));
		return 1;
	}
	mtQuery(pParam);
	return 0;
}

BOOL mtUpload(CString strUrl, CString strName, CString strPwd, CString strSourcecName, CString strDestName, CString strCurDir)
{
	CInternetSession* pSession = nullptr;
	pSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);
	CFtpConnection* pConnection = nullptr;
	try
	{
		pConnection = pSession->GetFtpConnection(strUrl, strName, strPwd, 21, TRUE);
	}
	catch (CInternetException * e)
	{
		e->Delete();
		pConnection = nullptr;
		return 1;
	}
	if (pConnection != nullptr)
	{
		pConnection->SetCurrentDirectory(strCurDir);
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
}

BOOL mtGetFile(CString strSourceName, CString strDestName, CFtpConnection* pConnection)
{
	CInternetFile* cSrcFile = pConnection->OpenFile(strSourceName);
	CFile cDestFile(strDestName, CFile::modeNoTruncate | CFile::modeWrite);
	cDestFile.SeekToEnd();
	auto nDestSize = cDestFile.GetLength();
	auto nSrcSize = cSrcFile->GetLength();
	cSrcFile->Seek(nDestSize, CFile::begin);
	char* pBuf[1024];
	while (nSrcSize > nDestSize)
	{
		cSrcFile->Read(pBuf, 512);
		cDestFile.Write(pBuf, 512);
		nDestSize = cDestFile.GetLength();
	}
	return 0;
}

UINT mtUpdateProgress(LPVOID pParam)
{
	if (pParam == NULL)
		AfxEndThread(NULL);
	PROGRESS_INFO* PP = (PROGRESS_INFO*)pParam;
	CProgressCtrl* pcProgress = PP->pcProgress;
	CFileStatus status;
	FILE_INFO fileInfo = PP->fileInfo;
	CString strDName = fileInfo.strDName;
	CString* strPercent = PP->strPercent;
	int ftpFileSize = fileInfo.nFileSize;
	int lSizeOfFile = 0;
	CFile::GetStatus(strDName, status);
	CString str;
	while (lSizeOfFile < ftpFileSize)
	{
		int pro = lSizeOfFile * 100 / ftpFileSize;
		pcProgress->SetPos(pro);
		strPercent->Format(L"%d%%", pro);
		AfxGetMainWnd()->PostMessage(WM_UPDATE_PROGESS);
		CFile::GetStatus(strDName, status);
		lSizeOfFile = status.m_size;
	}
	*strPercent = TEXT("100%");
	AfxGetMainWnd()->PostMessage(WM_UPDATE_PROGESS);
	return 0;
}

