#pragma once
UINT mtQuery(LPVOID pParam);
UINT mtDownloadFile(LPVOID pParam);
UINT mtUploadFile(LPVOID pParam);
BOOL mtDownLoad(CString strFtpSite, CString strName, CString strPwd, CString strSourceName, CString strDestName);
UINT mtUploadFile(LPVOID pParam);
BOOL mtUpload(CString strUrl, CString strName, CString strPwd, CString strSourcecName, CString strDestName);