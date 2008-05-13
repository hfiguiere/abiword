/* Copyright (C) 2006-2008 by Marc Maurer <uwog@uwog.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#ifndef __TCPACCOUNTHANDLER__
#define __TCPACCOUNTHANDLER__

#include <boost/shared_ptr.hpp>
#include <backends/xp/AccountHandler.h>

#include "IOServerHandler.h"

class TCPBuddy;

#define DEFAULT_TCP_PORT 25509  /* log2(e + pi) * 10^4 */

extern AccountHandlerConstructor TCPAccountHandlerConstructor;

class  TCPAccountHandler : public AccountHandler
{
public:
	TCPAccountHandler();
	virtual ~TCPAccountHandler();

	// housekeeping
	virtual UT_UTF8String					getDescription();
	virtual UT_UTF8String					getDisplayType();
	virtual UT_UTF8String					getStorageType();
	
	// dialog management 
	virtual void							storeProperties();

	// connection management
	virtual ConnectResult					connect();
	virtual bool							disconnect();
	virtual bool							isOnline();

	// user management
	virtual Buddy*							constructBuddy(const PropertyMap& props);
	virtual bool							allowsManualBuddies()
		{ return false; }
	virtual void							forceDisconnectBuddy(Buddy* buddy);
		
	// packet management
	virtual bool							send(const Packet* packet);
	virtual bool							send(const Packet*, const Buddy& buddy);

	// event management
	void									handleEvent(Session& session);

protected:

	// connection management
	virtual UT_sint32						_getPort(const PropertyMap& props);

	// user management
	const TCPBuddy*							_getBuddy(Session* pSession);
	
private:
	void									_handleAccept(IOServerHandler* pHandler, boost::shared_ptr<Session> session);
	void									_teardownAndDestroyHandler();
	void									_handleMessages(Session& session);
		
		
	asio::io_service						m_io_service;
	asio::io_service::work					m_work;
	asio::thread*							m_thread;
	bool									m_bConnected; // TODO: drop this, ask the IO handler
	IOServerHandler*						m_pDelegator;

	std::map<const TCPBuddy*, boost::shared_ptr<Session> >		m_clients; // mapping buddies and their accompanying session
};

#endif /* __TCPACCOUNTHANDLER__ */
