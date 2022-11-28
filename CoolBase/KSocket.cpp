#include "stdafx.h"
#include "../include/base/ksocket.h"

#ifdef WIN32
#include <WinSock2.h>
#endif

KTcpSocket::KTcpSocket()
{	
	m_socket = INVALID_SOCKET;
}

KTcpSocket::~KTcpSocket()
{
}

int	KTcpSocket::GetSocket()
{
	return m_socket;
}

int KTcpSocket::operator ()()
{
	return m_socket;
}

void KTcpSocket::Attach(int sock)
{
	Close();
	m_socket = sock;
}

int	KTcpSocket::Detch()
{
	int tmp = m_socket;
	m_socket = 0;

	return tmp;
}

void KTcpSocket::SetAddr(KString strip, unsigned short port)
{
	m_ip = strip;
	m_port = port;
}

void KTcpSocket::GetAddr(KString* strip, unsigned short* port)
{
	if (strip)
		*strip = m_ip;
	if (port)
		*port = m_port;
}

bool KTcpSocket::IsConnected()
{
	if (m_socket != INVALID_SOCKET)
		return true;

	return false;
}

bool KTcpSocket::IsListening()
{
	if (m_socket != INVALID_SOCKET)
		return true;

	return false;
}

int KTcpSocket::Recv(void* buffer, int buflen, int flag)
{
	return recv(m_socket, (char*)buffer, buflen, flag);
}

int KTcpSocket::Send(void* buffer, int datalen, int flag)
{
	return send(m_socket, (char*)buffer, datalen, flag);
}

bool KTcpSocket::Close()
{
	if (m_socket)
	{
		shutdown(m_socket, 2);
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	return true;
}

bool KTcpSocket::Connect()
{
	Close();

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET) 
		return false;

	KStringA ip = m_ip;

	sockaddr_in clientService; 
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(ip.GetData());
	clientService.sin_port = htons(m_port);

	if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		return false;
	}

	return true;
}

bool KTcpSocket::Listen()
{
	Close();

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET) 
	{	
		return false;
	}

	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	sockaddr_in service;
	service.sin_family = AF_INET;
	if (m_ip.GetLength() > 0)
	{
		KStringA ip = m_ip;
		service.sin_addr.s_addr = inet_addr(ip.GetData());
	}
	else
	{
		service.sin_addr.s_addr = INADDR_ANY;
	}
	service.sin_port = htons(m_port);

	if (bind(m_socket, (SOCKADDR*) &service, sizeof(service)) == SOCKET_ERROR) 
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		return false;
	}

	//----------------------
	// Listen for incoming connection requests 
	// on the created socket
	if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////
KNetWork::KNetWork()
{
	m_soEvent = 0 ;
	m_listenSock = 0;
	m_bRecving = false;
}

KNetWork::KNetWork(KTcpSocketEvent* pEvent)
{
	m_soEvent = pEvent;
	m_listenSock = 0;
	m_bRecving = false;
}

KNetWork::~KNetWork()
{
	// user programe must call uninit().
}

KTcpSocket* KNetWork::CreateSocket()
{
	return new KTcpSocket();
}

bool KNetWork::Init()
{
#ifdef WIN32
    WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return false;
#endif
	return true;
}

void KNetWork::Uninit()
{
	if (m_listenSock)
	{
		m_listenSock->Close();
		m_listenSock->Release();
		m_listenSock = 0;
	}
	StopTCPServer();
	StopRecvThread();

	RemoveSocketAll();
#ifdef WIN32
	WSACleanup();
#endif
}

static void KTcpSocket_Listen_Proc(void* pVoid)
{
	KNetWork* pThis = (KNetWork*)pVoid;
	pThis->DoListenThread();
}

bool KNetWork::StartTCPServer(unsigned short port)
{
	StopTCPServer();
	
	m_thdListen.SetThreadProc(KTcpSocket_Listen_Proc, this);

	if (0 == m_listenSock)
	{
		m_listenSock = CreateSocket();
		m_listenSock->SetAddr(L"", port);
	}

	if (0 == m_listenSock)
		return false;

	if (!m_listenSock->Listen())
	{
		m_listenSock->Release();
		m_listenSock = 0;
		return false;
	}

	m_thdListen.Start();

	return true;
}

void KNetWork::StopTCPServer()
{
	if (m_listenSock)
	{
		KTcpSocket* t = m_listenSock;
		m_listenSock = 0;

		t->Close();
		m_thdListen.WaitThreadEnd(-1);
		t->Release();		
	}
}

static void KTcpSocket_recv_Proc(void* pVoid)
{
	KNetWork* pThis = (KNetWork*)pVoid;
	pThis->DoRecvThread();
}

bool KNetWork::StartRecvThread()
{
	StopRecvThread();

	m_thdRecving.SetThreadProc(KTcpSocket_recv_Proc, this);
	m_bRecving = true;
	if (!m_thdRecving.Start())
		m_bRecving = false;

	return m_bRecving;
}

void KNetWork::StopRecvThread()
{
	m_bRecving = false;
	m_thdRecving.WaitThreadEnd(-1);
}

int KNetWork::GetSocketCount()
{
	return m_sockets.GetCount();
}

bool KNetWork::BroadMsg(void* data, int datalen)
{
	KThreadLockAuto acs(&m_lock);
	for(int i=0 ; i<m_sockets.GetCount(); i++)
	{
		KTcpSocket* peer = (KTcpSocket*)m_sockets.GetAt(i);
		peer->Send(data, datalen);
	}
	return true;
}

int KNetWork::AddSocket(KTcpSocket* pSo)
{
	if (0 == pSo)
		return 0;

	KThreadLockAuto acs(&m_lock);
	return m_sockets.Add(pSo);
}

void KNetWork::RemoveSocket(KTcpSocket* pSo)
{
	if (0 == pSo)
		return;

	KThreadLockAuto acs(&m_lock);
	int index = m_sockets.FindIndex(pSo);
	if (index >= 0)
		m_sockets.RemoveAt(index);

	pSo->Release();
}

void KNetWork::RemoveSocketAll()
{
	KThreadLockAuto acs(&m_lock);
	for(int i=0 ; i<m_sockets.GetCount(); i++)
	{
		KTcpSocket* peer = (KTcpSocket*)m_sockets.GetAt(i);
		peer->Release();
	}
	m_sockets.RemoveAll();
}

void KNetWork::DoListenThread()
{
	SOCKET so = m_listenSock->GetSocket();
	while(1)
	{
		SOCKET AcceptSocket;
		SOCKADDR_IN addr = {0};
		int addrlen = sizeof(SOCKADDR_IN);
		//----------------------
		// Accept the connection.
		AcceptSocket = accept(m_listenSock->GetSocket(), (sockaddr*)(&addr), &addrlen);
		if (AcceptSocket == INVALID_SOCKET) 
		{
			continue;
		}

		KString strip = inet_ntoa(addr.sin_addr);
		KTcpSocket *pNew = CreateSocket();
		pNew->SetAddr(strip, addr.sin_port);
		pNew->m_socket = AcceptSocket;

		if (m_soEvent)
		{
			if (!m_soEvent->OnAccepted(pNew))
			{
				pNew->Release();
				pNew = 0;
			}
		}

		AddSocket(pNew);
	}
}

void KNetWork::DoRecvThread()
{
	KArrayInt arr;
	while(m_bRecving)
	{
		arr.RemoveAll();
		_PrepareRecvSocket(&arr);

		if (arr.GetCount() == 0)
		{
			Sleep(20);
			continue;
		}

		FD_SET fs;
		FD_ZERO(&fs);

		int tmpindex = 0;
		for(int i=0; i<arr.GetCount(); i++)
		{
			FD_SET(arr.GetAt(i), &fs);
			tmpindex++;

			if (tmpindex == FD_SETSIZE)
			{
				fs.fd_count = FD_SETSIZE;
				_DoRecv(i - tmpindex, &fs);
				
				fs.fd_count = 0;
				tmpindex = 0;
			}
		}
		if (tmpindex > 0)
		{
			fs.fd_count = tmpindex;
			_DoRecv(arr.GetCount() - tmpindex, &fs);
		}
	}
}

void KNetWork::_PrepareRecvSocket(void* pArr)
{
	KArrayInt * p = (KArrayInt *)pArr;
	
	KTcpSocket* pso = (KTcpSocket*)m_removed.GetTail();
	while(pso)
	{
		pso->Close();
		m_soEvent->OnClosed(pso);

		RemoveSocket(pso);
		pso = (KTcpSocket*)m_removed.GetTail();
	}

	KThreadLockAuto acs(&m_lock);
	for(int i=0 ; i<m_sockets.GetCount(); i++)
	{
		KTcpSocket* peer = (KTcpSocket*)m_sockets.GetAt(i);
		int s = peer->GetSocket();
		p->Add(s);
	}
}

void KNetWork::_DoRecv(int baseindex, void* pFDSET)
{
	FD_SET* fs = (FD_SET *)pFDSET;
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 10;
	int t = select(0, fs, 0, 0, &timeout);

	KBuffer buffer;
	buffer.SetSize(4096);
	for(unsigned int i=0; i<fs->fd_count; i++)
	{
		KTcpSocket* peer = (KTcpSocket*)m_sockets.GetAt(i + baseindex);
		if (!peer->IsConnected())
			continue;

		if (FD_ISSET(peer->GetSocket(), fs))
		{
			buffer.Clear();
			int t = peer->Recv(buffer.GetData(), 4096, 0);

			if (t <= 0)
			{
				m_removed.Add(peer);
			}
			else
			{
				m_soEvent->OnRecvData(peer, buffer.GetData(), t);
			}
		}
	}
}