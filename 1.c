#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib") // ����Winsock��

#define PORT 8888
#define BUFFER_SIZE 1024
#define TRUE 1

typedef struct {
    char classroom[10];
    char date[11];
    char startTime[6];
    char endTime[6];
} Booking;

int send_response(SOCKET socket, const char* content_type, const char* body, int body_len) {
    char headers[256];
    sprintf_s(headers, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n", content_type, body_len);
    send(socket, headers, strlen(headers), 0);
    send(socket, body, body_len, 0);
    return 1;
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in server, client;
    char buffer[BUFFER_SIZE];
    int recv_size, addr_size = sizeof(struct sockaddr_in);

    // ��ʼ��Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }

    // �����������׽���
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // ���׽���
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = (ULONG)0x00000000;
    server.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // ��������
    listen(serverSocket, 3);

    // ��������
    printf("Waiting for incoming connections...\n");
    while ((clientSocket = accept(serverSocket, (struct sockaddr*)&client, &addr_size)) != INVALID_SOCKET) {
        printf("Connection accepted\n");

        // ��������
        recv_size = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (recv_size == SOCKET_ERROR) {
            printf("recv failed with error code : %d", WSAGetLastError());
        }
        buffer[recv_size] = '\0'; // ȷ���ַ�����NULL��β

        // ����HTTP����
        if (strstr(buffer, "POST /addBooking") != NULL) {
            // ����������
            // ������Ҫʵ�ֽ��������ݵ��߼�
            // ...

            // ������Ӧ
            const char* response = "Booking added";
            send_response(clientSocket, "text/plain", response, strlen(response));
        }
        else if (strstr(buffer, "POST /deleteBooking") != NULL) {
            // ����������
            // ������Ҫʵ�ֽ��������ݵ��߼�
            // ...

            // ������Ӧ
            const char* response = "Booking deleted";
            send_response(clientSocket, "text/plain", response, strlen(response));
        }
        else {
            // ����404��Ӧ
            const char* response = "404 Not Found";
            send_response(clientSocket, "text/plain", response, strlen(response));
        }

        closesocket(clientSocket);
    }

    // ����
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}