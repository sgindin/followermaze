/* This file defines the Client interface and it's specialisation Admin.
 * Client is a base class for all clients. Client implements EventHandler
 * interface to introduce basic functionality and error handling.
 * Client encapsulates a Connection and uses Reactor to handle events.
 * Client instances are created by Acceptor, owned by Reactor and disposed
 * of in call back functions while hadling client disconnect or error.
 * Admin is a special type of client used to interrupt Reactor's event loop.
 */

#ifndef CLIENT_H
#define CLIENT_H

#include "eventhandler.h"
#include "connection.h"
#include <memory>
#include <string>

namespace followermaze
{

class Reactor;
class Engine;

class Client : public EventHandler
{
public:
    Client(auto_ptr<Connection> conn, Reactor &reactor);

    // Implement EventHandler  interface.
    virtual Handle getHandle();
    virtual void handleInput(int hint);
    virtual void handleOutput(int hint);
    virtual void handleClose(int hint);
    virtual void handleError(int hint);

protected:
    // handleInput and handleOutput are implemented as template methods
    // which define error handling around doHandleInput call which can
    // be overridden by the subclasses do the processing.
    virtual void doHandleInput(int hint);
    virtual void doHandleOutput(int hint);

    // Unregister itself from the Reactor and dispose of itself.
    void disconnect(int hint);

protected:
    // Ensure dynamic allocation.
    virtual ~Client();

protected:
    auto_ptr<Connection> m_conn;
    Reactor &m_reactor;
};

class Admin : public Client
{
public:
    Admin(auto_ptr<Connection> conn, Reactor &reactor);

protected:
    // Expect "stop". On receiving stop Reactor.
    virtual void doHandleInput(int hint);

protected:
    // Ensure dynamic allocation.
    virtual ~Admin();
};

} // namespace followermaze

#endif // CLIENT_H
