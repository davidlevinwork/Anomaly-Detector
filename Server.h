#ifndef EVEN_DERECH_6_SERVER_H
#define EVEN_DERECH_6_SERVER_H
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "Server.h"

using namespace std;


class ClientHandler{
public:
    virtual void handle(int clientID)=0;
};


class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID)
    {
        char buffer[1024];
        // bzero
        int numOfBytes = read(clientID, buffer, 100); // change the 100
        cout << buffer << endl;
        const char* hello = "hello from server"; // message return from the server to client
        send(clientID, hello, strlen(hello), 0);
    }
};



class Server{
    thread* t; // the thread to run the start() method in
    int fileDescriptor;
    int limitClients;
    bool stopSign;
    sockaddr_in server;
    sockaddr_in client;

public:
    Server(int port) throw (const char*);
    virtual ~Server();
    void start(ClientHandler& ch) throw (const char*);
    void stop();
};

#endif //EVEN_DERECH_6_SERVER_H
