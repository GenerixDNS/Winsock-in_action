#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#include <cstdio>
#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
#include <inaddr.h>

/*
 * Inter process communication
 */

using namespace std;

int main() {

    /*
     * Hilfsvariable
     * Speichert Rückgabewerte von Funktionen
     */
    long res;

    /*
     * Versionsdaten
     */
    WSAData data; // NOLINT(cppcoreguidelines-pro-type-member-init)

    /*
     * ws2_32.dll aktivieren (lib)
     */
    res = WSAStartup(MAKEWORD(2, 0), &data);

    if (res == 0)
    {
        cout << "Startup succesfull" << endl;
    } else
    {
        cout << "error startup: " << WSAGetLastError() << endl;
    }

    /*
     * Sockets (server)
     */
    SOCKET serverSock;

    /*
     * Sockets (futureClient)
     */
    SOCKET clientSock;

    /*
     * Kommunikationspunkt einrichten
     *
     * (1) - Address family | IPV4(selected)/IPV6
     * (2) - Übertragungsart | SOCK_STREAM(TCP), SOCK_DGRAM(UDP)
     * (3) - Portockoll | 0 = Standart
     *
     */
    serverSock = socket(AF_INET, SOCK_STREAM, 0);

    /*
     * check socket
     */
    if (serverSock != INVALID_SOCKET)
    {
        cout << "socket succesfully inserted" << endl;
    } else
    {
        cout << "invalid socket: " << WSAGetLastError() << endl;
    }

    /*
     * Verbindungsinformationen
     *
     * sockaddr_in || sockaddr
     * sin_family Address Family
     *
     */
    sockaddr_in info;
    info.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    info.sin_family = AF_INET;
    info.sin_port = htons(2000);
    int infolen = sizeof(info);

    /*
     * server an einen Port binden
     */
    res = bind(serverSock, (struct sockaddr*)&info, infolen);

    /*
     * check bind
     */
    if (res != SOCKET_ERROR)
    {
        cout << "bind successfully" << endl;
    } else
    {
        cout << "error bind: " << WSAGetLastError() << endl;
    }

    /*
     * Warteschlange für einkommende Verbindungen
     *
     * (2) - Max Anzahl an Verbindungen
     *
     */
    res = listen(serverSock, SOMAXCONN);

    /*
     * check listen
     */
    if (res != SOCKET_ERROR)
    {
        cout << "listen successfully" << endl;
    } else
    {
        cout << "error listen: " << WSAGetLastError() << endl;
    }

    /*
     * Struktur für Client festlegen
     */
    sockaddr_in clientInfo;
    int clientinfolen = sizeof(clientInfo);

    /*
     * main Schleife
     */
    while (1)
    {
        clientSock = accept(serverSock, (struct sockaddr*)&clientInfo, &clientinfolen);
        if (clientSock != SOCKET_ERROR)
        {
            cout << "client successfully accepted (" << ntohs(clientInfo.sin_port) << ") (" << inet_ntoa(clientInfo.sin_addr) << ")" << endl;
        }

    }

    closesocket(clientSock);
    closesocket(serverSock);
    WSACleanup();
    return 0;
}