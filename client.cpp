#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <netinet/in.h>
#include "input.h"
using namespace std;

int main() {

    char buff[4096];
    string input;
    do {
        input[0]='\0';
        cout << "Enter data: ";
        getline(cin, input);
        Input data=Input(input);

        // Create a socket
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            cerr << "Fail to create socket";
            return 1;
        }
        // Create a hint structure for the server
        string ipAddr = "127.0.0.1";
        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(data.port);
        inet_pton(AF_INET, ipAddr.c_str(), &hint.sin_addr);

        // Connect to the server via the socket
        int connRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
        if (connRes == -1) {
            cerr << "Fail to connect to server";
            return 2;
        }
        // Send to server
        int sendRes = send(sock, input.c_str(), input.size() + 1, 0);
        if (sendRes == -1) {
            cout << "Fail to send to server\r\n";
            continue;
        }
        // Wait for response
        memset(buff, 0, 4096);
        int bytesRecv = recv(sock, buff, 4096, 0);
        if (bytesRecv == -1)
            cout << "Fail to get response from server\r\n";
        else if(input[0]!='\0')
            cout << "Server response > " << string(buff, bytesRecv) << "\n"; // Display response
        sleep(data.period);
        close(sock);
    } while(true);
    return 0;
}