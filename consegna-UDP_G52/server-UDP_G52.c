#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORTA 4000

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server, client;
    int lunghezza = sizeof(client);

    char lettera;
    char buffer[100];
    int x, y, risultato;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Errore WSAStartup\n");
        return 1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Errore socket\n");
        WSACleanup();
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORTA);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Errore bind\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("SERVER UDP in ascolto sulla porta %d...\n", PORTA);

    while (1) {
        recvfrom(sock, (char*)&lettera, sizeof(lettera), 0,
                 (struct sockaddr *)&client, &lunghezza);

        switch(lettera) {
            case 'a': case 'A': strcpy(buffer, "ADDIZIONE"); break;
            case 's': case 'S': strcpy(buffer, "SOTTRAZIONE"); break;
            case 'm': case 'M': strcpy(buffer, "MOLTIPLICAZIONE"); break;
            case 'd': case 'D': strcpy(buffer, "DIVISIONE"); break;
            default: strcpy(buffer, "TERMINE PROCESSO CLIENT"); break;
        }

        sendto(sock, buffer, strlen(buffer) + 1, 0,
               (struct sockaddr *)&client, lunghezza);

        if (strcmp(buffer, "TERMINE PROCESSO CLIENT") != 0) {

            recvfrom(sock, (char*)&x, sizeof(int), 0,
                     (struct sockaddr *)&client, &lunghezza);
            recvfrom(sock, (char*)&y, sizeof(int), 0,
                     (struct sockaddr *)&client, &lunghezza);

            switch(lettera) {
                case 'A': case 'a': risultato = x + y; break;
                case 'S': case 's': risultato = x - y; break;
                case 'M': case 'm': risultato = x * y; break;
                case 'D': case 'd': risultato = (y != 0 ? x/y : 0); break;
            }

            sendto(sock, (char*)&risultato, sizeof(int), 0,
                   (struct sockaddr *)&client, lunghezza);
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}

