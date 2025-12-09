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
    struct sockaddr_in server;
    struct hostent *host;

    char nomeserver[100];
    char buffer[100];
    char lettera;
    int x, y, risultato;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        return 1;
    }

    printf("Inserire il nome del server: ");
    scanf("%s", nomeserver);

    host = gethostbyname(nomeserver);
    if (!host) {
        printf("Server non trovato.\n");
        WSACleanup();
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORTA);

    memcpy(&server.sin_addr.s_addr, host->h_addr, host->h_length);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Connessione fallita.");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    recv(sock, buffer, sizeof(buffer), 0);
    printf("SERVER: %s\n", buffer);

    printf("Inserire una lettera tra A,S,M,D: ");
    scanf(" %c", &lettera);
    send(sock, &lettera, sizeof(lettera), 0);

    recv(sock, buffer, sizeof(buffer), 0);
    printf("SERVER: %s\n", buffer);

    if (strcmp(buffer, "TERMINE PROCESSO CLIENT") == 0) {
        closesocket(sock);
        WSACleanup();
        return 0;
    }

    printf("Inserire il primo intero: ");
    scanf("%d", &x);
    printf("Inserire il secondo intero: ");
    scanf("%d", &y);

    send(sock, (char*)&x, sizeof(int), 0);
    send(sock, (char*)&y, sizeof(int), 0);

    recv(sock, (char*)&risultato, sizeof(int), 0);
    printf("Risultato: %d\n", risultato);

    closesocket(sock);
    WSACleanup();
    return 0;
}

