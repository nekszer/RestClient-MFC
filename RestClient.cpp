#include "stdafx.h"
#include "RestClient.h"

using namespace RestClient;

Rest::Rest()
{
}


Rest::~Rest()
{
}

string RestClient::Rest::Request(string _server, string _endpoint, int _http_method, string _data, int _nPort)
{
	CString server(_server.c_str());
	CString endpoint(_endpoint.c_str());
	CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded");
	LPCTSTR pstrServer = (LPCTSTR) server; //_T4(server);
	INTERNET_PORT nPort = _nPort;
	CInternetSession session(_T("SessionApp"));
	string sFormData = _data;

	char *pFormData = new char[sFormData.size() + 1];
	memcpy(pFormData, sFormData.data(), sFormData.size());

	CHttpConnection *pConnection = session.GetHttpConnection(pstrServer, nPort, NULL, NULL);

	CHttpFile *pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, (LPCTSTR) endpoint); // _T("angular/customers.php")
	pFile->AddRequestHeaders(strHeaders);
	BOOL result = pFile->SendRequest(strHeaders, (LPVOID)(char *)pFormData, sFormData.size());

	DWORD dwRet;
	pFile->QueryInfoStatusCode(dwRet);
	
	CString out;
	if (dwRet == HTTP_STATUS_OK)
	{
		char szBuff[1024];
		UINT nRead = pFile->Read(szBuff, 1023);
		while (nRead > 0)
		{
			string result = "";
			for (int i = 0; i < nRead; i++) {
				result += szBuff[i];
			}
			out = CString(result.c_str());
			break;
		}
	}
	free(pFormData);
	session.Close();

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(out);
	// construct a std::string using the LPCSTR input
	string response(pszConvertedAnsiString);
	return response;
}

string RestClient::Rest::RequestMultipart(string _server, string _endpoint, int _http_method, string _data, int _nPort, string _urlpathfile)
{
	CString urlpathfile(_urlpathfile.c_str());
	CString server(_server.c_str());
	CString endpoint(_endpoint.c_str());

	INTERNET_PORT nPort = _nPort;
	CInternetSession ises(_T("SessionApp"));
	CHttpFile* httpf = NULL;
	CHttpConnection *connection = NULL;

	CFile file(urlpathfile, CFile::modeRead);

	connection = ises.GetHttpConnection(server, nPort, NULL, NULL);
	CHttpFile *pFile = connection->OpenRequest(_http_method, (LPCTSTR)endpoint);
	httpf = connection->OpenRequest(_http_method, endpoint, 0, 1, 0, 0, INTERNET_FLAG_KEEP_CONNECTION);

	CString strHTTPBoundary = _T("FFF3F395A90B452BB8BEDC878DDBD152");
	CString hdrs = MakeRequestHeaders(strHTTPBoundary); /* = My appropriate headers to send */;
	httpf->AddRequestHeaders(hdrs);
	CString PreFileData = MakePreFileData(strHTTPBoundary, file.GetFileName());
	CString PostFileData = MakePostFileData(strHTTPBoundary);

	httpf->SendRequestEx(DWORD(PreFileData.GetLength() + file.GetLength() + PostFileData.GetLength()), HSR_SYNC | HSR_INITIATE);
	httpf->Write((LPSTR)(LPCSTR)CW2A(PreFileData), PreFileData.GetLength());

	UINT len = 1024;
	char buf[1024]; /*BUFFER SIZE = 1024 HOW BIG ACTUALLY SHOULD IT BE?*/
	while (len > 0) {
		len = file.Read(&buf, 1024);
		if (len>0)httpf->Write(&buf, len);
	}

	httpf->Write((LPSTR)(LPCSTR)CW2A(PostFileData), PostFileData.GetLength());

	httpf->EndRequest(HSR_SYNC);

	DWORD dwRet;
	pFile->QueryInfoStatusCode(dwRet);

	CString out;
	if (dwRet == HTTP_STATUS_OK)
	{
		char szBuff[1024];
		UINT nRead = pFile->Read(szBuff, 1023);
		while (nRead > 0)
		{
			string result = "";
			for (int i = 0; i < nRead; i++) {
				result += szBuff[i];
			}
			out = CString(result.c_str());
			break;
		}
	}


	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(out);
	// construct a std::string using the LPCSTR input
	string response(pszConvertedAnsiString);

	file.Close();
	httpf->Close();
	connection->Close();
	return response;
}

/*
string Rest::Rest::RequestMultipart(string _server, string _endpoint, int _http_method, string _data, int _nPort, string _urlpathfile)
{
	
	CString server(_server.c_str());
	CString endpoint(_endpoint.c_str());
	CString strHeaders = _T("Content-Type: multipart/form-data; boundary=FFF3F395A90B452BB8BEDC878DDBD152");
	LPCTSTR pstrServer = (LPCTSTR)server; //_T4(server);
	INTERNET_PORT nPort = _nPort;
	CInternetSession session(_T("SessionApp"));
	string sFormData = _data;

	char *pFormData = new char[sFormData.size() + 1];
	memcpy(pFormData, sFormData.data(), sFormData.size());

	CHttpConnection *pConnection = session.GetHttpConnection(pstrServer, nPort, NULL, NULL);

	CHttpFile *pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, (LPCTSTR)endpoint); // _T("angular/customers.php")
	pFile->AddRequestHeaders(strHeaders);
	BOOL result = pFile->SendRequest(strHeaders, (LPVOID)(char *)pFormData, sFormData.size());

	DWORD dwRet;
	pFile->QueryInfoStatusCode(dwRet);

	CString out;
	if (dwRet == HTTP_STATUS_OK)
	{
		char szBuff[1024];
		UINT nRead = pFile->Read(szBuff, 1023);
		while (nRead > 0)
		{
			string result = "";
			for (int i = 0; i < nRead; i++) {
				result += szBuff[i];
			}
			out = CString(result.c_str());
			break;
		}
	}
	free(pFormData);
	session.Close();

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(out);
	// construct a std::string using the LPCSTR input
	string response(pszConvertedAnsiString);
	return response;
}
*/

CString Rest::MakeRequestHeaders(CString& strBoundary)
{
	CString strFormat;
	CString strData;
	strFormat = _T("Content-Type: multipart/form-data; boundary=%s\r\n");
	strData.Format(strFormat, strBoundary);
	return strData;
}


CString Rest::MakePreFileData(CString& strBoundary, CString& strFileName)
{
	CString strFormat;
	CString strData;

	strFormat += _T("--%s");
	strFormat += _T("\r\n");
	strFormat += _T("Content-Disposition: form-data; name=\"file\"; filename=\"%s\"");
	strFormat += _T("\r\n");
	strFormat += _T("Content-Type: text/plain");
	strFormat += _T("\r\n");
	strFormat += _T(" XXXXX ");
	strFormat += _T("\r\n\r\n");

	strData.Format(strFormat, strBoundary,/* m_Name , */strBoundary, strFileName);

	return strData;
}

CString Rest::MakePostFileData(CString& strBoundary)
{

	CString strFormat;
	CString strData;

	strFormat = _T("\r\n");
	strFormat += _T("--%s");
	strFormat += _T("\r\n");
	strFormat += _T("Content-Disposition: form-data; name=\"submit\"");
	strFormat += _T("\r\n\r\n");
	strFormat += _T("");
	strFormat += _T("\r\n");
	strFormat += _T("--%s--");
	strFormat += _T("\r\n");

	strData.Format(strFormat, strBoundary, strBoundary);

	return strData;

}

/*
void Rest::UploadByPost(CString strFileName, CString  strServerUrl, CString strServerUploadFile, int _nPort)
{
	INTERNET_PORT nPort = _nPort;
	DWORD dwTotalRequestLength;
	DWORD dwChunkLength;
	DWORD dwReadLength;
	DWORD dwResponseLength;
	CHttpFile* pHTTP = NULL;

	dwChunkLength = 64 * 1024;
	void* pBuffer = malloc(dwChunkLength);
	CFile file;

	if (!file.Open(strFileName, CFile::modeRead | CFile::shareDenyWrite))
	{
		return;
	}

	CInternetSession session(L"sendFile");
	CHttpConnection *connection = NULL;

	try
	{
		//Create the multi-part form data that goes before and after the actual file upload.

		CString strHTTPBoundary = _T("FFF3F395A90B452BB8BEDC878DDBD152");
		CString strPreFileData = MakePreFileData(strHTTPBoundary, file.GetFileName());
		CString strPostFileData = MakePostFileData(strHTTPBoundary);
		CString strRequestHeaders = MakeRequestHeaders(strHTTPBoundary);
		dwTotalRequestLength = strPreFileData.GetLength() + strPostFileData.GetLength() + file.GetLength();

		connection = session.GetHttpConnection(/*L"www.YOURSITE.com" strServerUrl.GetBuffer(), nPort, NULL, NULL);

		pHTTP = connection->OpenRequest(CHttpConnection::HTTP_VERB_POST, strServerUploadFile.GetBuffer());//_T("/YOUURL/submit_file.pl"));
		pHTTP->AddRequestHeaders(strRequestHeaders);
		pHTTP->SendRequestEx(dwTotalRequestLength, HSR_SYNC | HSR_INITIATE);

		//Write out the headers and the form variables
		pHTTP->Write((LPSTR)(LPCSTR)CW2A(strPreFileData.GetBuffer()), strPreFileData.GetLength());

		//upload the file.

		dwReadLength = -1;
		int length = file.GetLength(); //used to calculate percentage complete.
		while (0 != dwReadLength)
		{
			dwReadLength = file.Read(pBuffer, dwChunkLength);
			if (0 != dwReadLength)
			{
				pHTTP->Write(pBuffer, dwReadLength);
			}
		}

		file.Close();

		//Finish the upload.
		pHTTP->Write((LPSTR)(LPCSTR)CW2A(strPostFileData.GetBuffer()), strPostFileData.GetLength());
		pHTTP->EndRequest(HSR_SYNC);


		//get the response from the server.
		LPSTR szResponse;
		CString strResponse;
		dwResponseLength = pHTTP->GetLength();
		while (0 != dwResponseLength)
		{
			szResponse = (LPSTR)malloc(dwResponseLength + 1);
			szResponse[dwResponseLength] = '\0';
			pHTTP->Read(szResponse, dwResponseLength);
			strResponse += szResponse;
			free(szResponse);
			dwResponseLength = pHTTP->GetLength();
		}

		TRACE(L"%s", strResponse.GetBuffer());

		//close everything up.
		pHTTP->Close();
		connection->Close();
		session.Close();
	}
	catch (CInternetException* e)
	{
		TRACE(L"error: %d \n", e->m_dwError);
	}
	catch (CFileException* e)
	{
		TRACE(L"error: %d \n", e->m_cause);
	}
	catch (...)
	{
		TRACE(L" unexpected error");
	}
}
*/