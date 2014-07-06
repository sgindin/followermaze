/* This file describes Acceptor class template (extension of the Reactor pattern).
 * Acceptor is a special event handler which accepts connection requests
 * and creates clients (defined as template parameter) to handle the connection.
 */

#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "eventhandler.h"
#include "reactor.h"
#include "logger.h"

namespace followermaze
{

template < class ClientType >
class Acceptor : public EventHandler
{
public:
    Acceptor(int port, Reactor &reactor) :
        m_conn(port),
        m_reactor(reactor)
    {
    }

    virtual Handle getHandle()
    {
        return m_conn.getHandle();
    }

    // Accepts connection request, creates the client and registers it
    // with the reactor.
    virtual void handleInput(int /*hint*/)
    {
        auto_ptr<Connection> clientConn(m_conn.accept(true));
        auto_ptr<EventHandler> client(new ClientType(clientConn, m_reactor));

        m_reactor.addHandler(client, Reactor::EvntRead);

        client.release();

        Logger::getInstance().message("Client connected.");
    }

protected:
    // Ensure dynamic allocation.
    virtual ~Acceptor() {}

protected:
    Connection m_conn;
    Reactor &m_reactor;
};

} // namespace followermaze

#endif // ACCEPTOR_H
