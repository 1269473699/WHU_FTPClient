#pragma once
class FtpClientDlg;
UINT mtQuery(LPVOID pParam);
UINT mtDownloadFile(LPVOID pParam);
UINT mtUploadFile(LPVOID pParam);
BOOL mtDownLoad(CString strFtpSite, CString strName, CString strPwd, CString strSourceName, CString strDestName, CString strCurDir);
UINT mtUploadFile(LPVOID pParam);
BOOL mtUpload(CString strUrl, CString strName, CString strPwd, CString strSourcecName, CString strDestName, CString strCurDir);
BOOL mtGetFile(CString strSourceName, CString strDestName, CFtpConnection* pConnection);
UINT mtUpdateProgress(LPVOID pParam);
UINT mtUpdateProgressUpload(LPVOID pParam);