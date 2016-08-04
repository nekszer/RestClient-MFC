#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <afxinet.h>

using namespace std;

namespace RestClient {

	class Rest
	{
	public:
		Rest();
		~Rest();

		string Request(string url, string endpoint, int _http_method, string data, int _nPort = 80);
		string RequestMultipart(string _server, string _endpoint, int _http_method, string _data, int _nPort, string _urlpathfile);
		/*
		CString MakeRequestHeaders(CString & strBoundary);
		CString MakePreFileData(CString & strBoundary, CString & strFileName);
		CString MakePostFileData(CString & strBoundary);
		void UploadByPost(CString strFileName, CString strServerUrl, CString strServerUploadFile, int port);
		*/
	private: 
		CString MakeRequestHeaders(CString & strBoundary);
		CString MakePreFileData(CString & strBoundary, CString & strFileName);
		CString MakePostFileData(CString & strBoundary);
	};
}