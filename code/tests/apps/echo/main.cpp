#include <iostream>
#include <string>
#include <memory>
#include "connection.h"

using namespace std;
using namespace followermaze;

int main()
{
    try
    {
        Connection eventSourceListener(9090);
        auto_ptr<Connection> eventSource(eventSourceListener.accept());

        for (;;)
        {
            string message = eventSource->receive();
            cout << "Received a message: " << message.c_str() << flush;
            eventSource->send("Ack message: ");
            eventSource->send(message);
        }
    }
    catch (Connection::Exception e)
    {
        if (e.getErr() == Connection::Exception::ErrClientDisconnect)
        {
            cout << "Client disconnected. Shutting down." << endl;
        }
        else
        {
            cout << e.what() << ": " << e.getErr() << ". Shutting down." << endl;
        }
    }

    return 0;
}
