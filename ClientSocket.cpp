// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "NetChess.h"
#include "NetChessDoc.h"
#include "NetChessView.h"
#include "ClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket()
{
	m_length = 0;
	m_observerFlag = FALSE;
	m_clientId = 0;
	m_icsFlag = FALSE;
}

CClientSocket::~CClientSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

void CClientSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class	 
	if(m_observerFlag == FALSE)
	{
		AfxMessageBox("Connection closed");
		((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->SetClientSocket(NULL,m_icsFlag);
	}
	else
	{
		((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->RemoveFromObserverList(this);
		CString str = "Observer " + m_ipaddress + " Closed the connection";		
		//Remove from linked list;
		AfxMessageBox(str);
	}
	CAsyncSocket::OnClose(nErrorCode);
}

void CClientSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CClientSocket *pClientSocket = (CClientSocket*)((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->GetClientSocket(m_icsFlag);
	if(nErrorCode == 0)
	{
 			pClientSocket->AsyncSelect(FD_READ | FD_CONNECT| FD_CLOSE | FD_WRITE);
		//	this->AsyncSelect(FD_READ | FD_CONNECT| FD_CLOSE | FD_WRITE);
	}
 	else
	{
		//m_pClientSocket->AsyncSelect(FD_READ | FD_CONNECT| FD_CLOSE | FD_WRITE);
		CString str;
		str.Format("Could not be conneted to Server");
		//ected,%s %d %d",dlg.m_strIPAddress,dlg.m_port_nunber,GetLastError());
		AfxMessageBox(str);		
		delete pClientSocket;
		pClientSocket = NULL;
		((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->SetClientSocket(NULL,m_icsFlag);
		
	}

//	UINT i;
//	GetPeerName(m_ipaddress,i);
//	m_port = i;
	CAsyncSocket::OnConnect(nErrorCode);
}

void CClientSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	unsigned char* data= NULL;
	if(m_icsFlag == FALSE)
	{
		if(m_length > 0)
		{		 
			data = (unsigned char*)malloc(m_length+1);
			if(data == NULL)
			{
				m_length = 0;
				return;
			}
			memset(data,'\0',m_length+1);
			int bytesread = Receive(data,m_length);	 
			((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->HandleData(data,m_length,m_icsFlag);
			if(data != NULL)
			{
				free(data);
				data = NULL;
			}
			m_length = 0;
		}
		else
		{

			
			char byte[4]; 
			Receive(byte,4);
			memcpy(&m_length,byte,4);
		//m_length = ntohs(m_length);
		}
	}
	else
	{
			char byte[1000]; 
			memset(byte,'\0',1000);
			int bytesread = Receive(byte,1000);
			((CNetChessView*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveView())->HandleData((unsigned char *)byte,m_length,m_icsFlag);
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnSend(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnSend(nErrorCode);
}

int CClientSocket::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CAsyncSocket::Receive(lpBuf, nBufLen, nFlags);
}

int CClientSocket::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CAsyncSocket::Send(lpBuf, nBufLen, nFlags);
}

void CClientSocket::SetInfo(CString ipaddr,int port)
{
	m_ipaddress = ipaddr;
	m_port = port;
}

void CClientSocket::GetInfo(CString &ipaddr,int & port)
{
	ipaddr = m_ipaddress;
	port = m_port;
}
void CClientSocket::SetICSFlag(int flag)
{
	m_icsFlag = flag;
}




