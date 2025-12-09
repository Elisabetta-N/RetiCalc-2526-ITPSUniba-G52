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
    int lunghezza = sizeof(server);

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
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Inserisci una lettera tra A,S,M,D: ");
    scanf(" %c", &lettera);

    sendto(sock, (char*)&lettera, sizeof(lettera), 0,
           (struct sockaddr *)&server, lunghezza);

    recvfrom(sock, buffer, sizeof(buffer), 0,
             (struct sockaddr *)&server, &lunghezza);

    printf("SERVER: %s\n", buffer);

    if (strcmp(buffer, "TERMINE PROCESSO CLIENT") == 0) {
        closesocket(sock);
        WSACleanup();
        return 0;
    }

    printf("Inserisci primo intero: ");
    scanf("%d", &x);
    printf("Inserisci secondo intero: ");
    scanf("%d", &y);

    sendto(sock, (char*)&x, sizeof(int), 0,
           (struct sockaddr *)&server, lunghezza);

    sendto(sock, (char*)&y, sizeof(int), 0,
           (struct sockaddr *)&server, lunghezza);

    recvfrom(sock, (char*)&risultato, sizeof(int), 0,
             (struct sockaddr *)&server, &lunghezza);

    printf("Risultato: %d\n", risultato);

    closesocket(sock);
    WSACleanup();
    return 0;
}
