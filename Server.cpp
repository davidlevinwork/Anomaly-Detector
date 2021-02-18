
#include "Server.h"

Server::Server(int port) throw (const char*)
{
    limitClients = 2; // limit the number of clients
    stopSign = false; // initialize to true for accepting clients
    fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(fileDescriptor < 0)
        throw "socket failed";
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if(bind(fileDescriptor, (struct sockaddr*)&server, sizeof(server) < 0))
        throw "bind failure";

    if(listen(fileDescriptor, limitClients) < 0)
        throw "listen failure";
}

void Server::start(ClientHandler& ch) throw (const char*)
{
    t = new thread([&ch, this]() { // lambda expression
        cout <<"waiting for a client"<< endl;
        socklen_t clientSize = sizeof(client);
        int aClient = accept(fileDescriptor, (struct sockaddr*)&client, &clientSize);

        if(aClient < 0)
            throw "accept failure";

        cout <<"client connected"<< endl;
        ch.handle(aClient);
        cout <<"connection finished"<< endl;
        close(aClient);
        close(fileDescriptor);
    }) ;
}

void Server::stop()
{
    // stopSign = true;
    t->join(); // do not delete this!
}

Server::~Server() { }
