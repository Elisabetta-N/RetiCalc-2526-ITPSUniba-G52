#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORTA 4000

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    int server, client;
    struct sockaddr_in indirizzoServer, indirizzoClient;
    int lunghezzaC = sizeof(indirizzoClient);

    char lettera;
    int x, y, risultato;
    char buffer[200];

    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == INVALID_SOCKET) {
        printf("Errore apertura socket\n");
        return 1;
    }

    memset(&indirizzoServer, 0, sizeof(indirizzoServer));
    indirizzoServer.sin_family = AF_INET;
    indirizzoServer.sin_addr.s_addr = INADDR_ANY;
    indirizzoServer.sin_port = htons(PORTA);

    if (bind(server, (SOCKADDR *)&indirizzoServer, sizeof(indirizzoServer)) == SOCKET_ERROR) {
        printf("Errore bind\n");
        return 1;
    }

    listen(server, 5);
    printf("SERVER TCP in ascolto sulla porta %d...\n", PORTA);

    while (1) {
        client = accept(server, (SOCKADDR *)&indirizzoClient, &lunghezzaC);
        if (client == INVALID_SOCKET) {
            printf("Errore accept\n");
            continue;
        }

        strcpy(buffer, "connessione avvenuta");
        send(client, buffer, strlen(buffer)+1, 0);

        recv(client, &lettera, sizeof(lettera), 0);

        if (lettera=='A'||lettera=='a') strcpy(buffer,"ADDIZIONE");
        else if(lettera=='S'||lettera=='s') strcpy(buffer,"SOTTRAZIONE");
        else if(lettera=='M'||lettera=='m') strcpy(buffer,"MOLTIPLICAZIONE");
        else if(lettera=='D'||lettera=='d') strcpy(buffer,"DIVISIONE");
        else strcpy(buffer,"TERMINE PROCESSO CLIENT");

        send(client, buffer, strlen(buffer)+1, 0);

        if (strcmp(buffer, "TERMINE PROCESSO CLIENT") != 0) {
            recv(client, (char*)&x, sizeof(int), 0);
            recv(client, (char*)&y, sizeof(int), 0);

            if(lettera=='A'||lettera=='a') risultato = x+y;
            else if(lettera=='S'||lettera=='s') risultato = x-y;
            else if(lettera=='M'||lettera=='m') risultato = x*y;
            else if(lettera=='D'||lettera=='d') risultato = y!=0 ? x/y : 0;

            send(client, (char*)&risultato, sizeof(int), 0);
        }

        closesocket(client);
    }

    closesocket(server);
    WSACleanup();
    return 0;
}

