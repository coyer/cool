#pragma once
/*******************************************************************************************
CoolBase SDK file.
Copyright(C) CoyerStudio. All RightsReserved  
Header File : kcolor.h
Author : coyer@126.com
Create : 
Update : 2012-11-05     version 0.0.1
Detail : Support linux or windows socket.

*******************************************************************************************/


#include "kbase.h"
#include "kthread.h"
#include "kstring.h"

class KAPI KTcpSocket : public KRefBase
{
	friend class KNetWork;
public:
	KTcpSocket();
	~KTcpSocket();

	void	Attach(int sock);
	int		Detch();

	int		GetSocket();

	int operator ()();

	void	GetAddr(KString* strip, unsigned short* port);	
	void	SetAddr(KString strip, unsigned short port);

	virtual bool	Connect();
	virtual int		Recv(void* buffer, int buflen, int flag = 0);
	virtual int		Send(void* buffer, int datalen, int flag = 0);
	virtual bool	Close();
	virtual bool	Listen();

	bool	IsConnected();
	bool	IsListening();

protected:
	int				m_socket;
	KString			m_ip;
	unsigned short	m_port;
};

class KAPI KTcpSocketEvent
{
public:
	virtual void  OnConnected(KTcpSocket* peer) = 0;
	virtual void  OnClosed(KTcpSocket* peer) = 0;
	virtual void  OnRecvData(KTcpSocket* peer, const unsigned char* data, int datalen) = 0;
	//if this event return false, pSocket will be release, if return true, pSocket will add to recvive list.
	virtual bool  OnAccepted(KTcpSocket* peer) = 0;
};

class KAPI KNetWork
{
	KArrayPtr	m_sockets;
	KArrayPtr	m_removed;
	KThread		m_thdListen;
	KThread		m_thdRecving;
	KThreadLock	m_lock;

	KTcpSocket*	m_listenSock;
	KTcpSocketEvent*	m_soEvent;

	bool		m_bRecving;

	KNetWork();
public:
	KNetWork(KTcpSocketEvent* pEvent);
	~KNetWork();

	bool  Init();
	void  Uninit();

	bool  StartTCPServer(unsigned short port);
	void  StopTCPServer();

	bool  StartRecvThread();
	void  StopRecvThread();

	int   GetSocketCount();
	bool  BroadMsg(void* data, int datalen);

	//do not call pSo->Release(), if you add to my list.
	int		AddSocket(KTcpSocket* pSo);
	void	RemoveSocket(KTcpSocket* pSo);
	void	RemoveSocketAll();

	void	DoListenThread();
	void	DoRecvThread();

	virtual KTcpSocket*	CreateSocket();

protected:
	void	_PrepareRecvSocket(void* pArr);
	void	_DoRecv(int baseindex, void* pFDSET);
};