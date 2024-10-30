#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include "input.h"
using namespace std;
int main(){
    // Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1) {
        cerr << "Fail to create socket";
        return -1;
    }
    // Bind the ip address and port to a socket
    int port = -1;
    do{
        cout << "Enter port: ";
        cin >> port;
        if (port < 0 )
            cout << "Invalid port. Try again";
    } while(port<0);
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
    bind(listening, (sockaddr*)&hint, sizeof(hint));
    listen(listening, SOMAXCONN); // Tell that the socket is for listening

    while(true){
        sockaddr_in client;
        socklen_t clientSize = sizeof(client);
        int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
        if(clientSocket<0){
            cout << "Connection not accepted" << endl;
            continue;
        }
        char host[NI_MAXHOST]; // Client's remote name
        char service[NI_MAXSERV]; // The port the client is connected on
        memset(host, 0, NI_MAXHOST);
        memset(service, 0, NI_MAXSERV);

        if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
            cout << host << " connected on port " << service << endl;
        else {
            inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
            cout << host << " connected on port " << ntohs(client.sin_port) << endl;
        }
    
        char buff[4096];
        memset(buff, 0, 4096);

        // Wait for the client to send data
        int bytesRecv = recv(clientSocket, buff, 4096, 0);
        if (bytesRecv == -1)
            cerr << "Error while receiving data" << endl;
        else if (bytesRecv == 0)
            cout << "Client disconnected " << endl;
        else{
            cout << string(buff, 0, bytesRecv) << endl; //Display message sent by the client
            Input data(string(buff, 0, bytesRecv)); // Extracting the cLient name

            // Formatting the time
            time_t currentTime = time(0);
            auto ms = currentTime%1000;
            struct tm  tstruct;
            char timeArray[80];
            tstruct = *localtime(&currentTime);
            strftime(timeArray, sizeof(timeArray), "%Y-%m-%d %X", &tstruct);

            // Write to the log file
            ofstream file("log.txt", ios_base::app);
            file << "[" << timeArray << "." << ms << "] " << data.name << endl;
            file.close();
            cout << "[" << timeArray << "." << ms << "] " << data.name << endl;

            send(clientSocket, buff, bytesRecv + 1, 0); // Message back to the client
        }
        close(clientSocket);
    }
    close(listening);
    return 0;
}