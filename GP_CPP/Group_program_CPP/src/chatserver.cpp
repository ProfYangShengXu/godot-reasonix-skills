#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#pragma comment(lib, "ws2_32.lib")

#define MYPORT  12345
#define BACKLOG 10

std::vector<SOCKET> clients;
std::mutex clients_mutex;

void get_cur_time(char* time_str) {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);

    strftime(time_str,50,"[%Y-%m-%d %H:%M:%S]",timeinfo);
}

void broadcast_message(const char* message, SOCKET sender) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    
    for (SOCKET client : clients) {
        if (client != sender && client != INVALID_SOCKET) {
            send(client, message, strlen(message), 0);
        }
    }
}

void handle_client(SOCKET clientfd) {
    char buf[1024];
    int recvbytes;
    

    while (true) {
        memset(buf, 0, sizeof(buf));
        recvbytes = recv(clientfd, buf, sizeof(buf) - 1, 0);
        
        if (recvbytes <= 0) {
            break;
        }
        
        buf[recvbytes] = '\0';
        
        // 添加时间戳并打印
        char time_str[50];
        get_cur_time(time_str);
        printf("[%s] %s\n", time_str, buf);
        
        // 广播消息给其他客户端（包含时间戳）
        char broadcast_msg[1024];
        snprintf(broadcast_msg, sizeof(broadcast_msg), "[%s] %s", time_str, buf);
        broadcast_message(broadcast_msg, clientfd);
    }
    
    // 移除断开连接的客户端
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        auto it = std::find(clients.begin(), clients.end(), clientfd);
        if (it != clients.end()) {
            clients.erase(it);
        }
    }
    
    closesocket(clientfd);
    printf("Client disconnected. Total clients: %zu\n", clients.size());
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }
    
    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        WSACleanup();
        return 1;
    }
    
    // 设置SO_REUSEADDR选项
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes)) == SOCKET_ERROR) {
        std::cerr << "Setsockopt failed" << std::endl;
    }
    
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MYPORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    
    if (listen(sockfd, BACKLOG) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    
    std::cout << "Server started on port " << MYPORT << std::endl;
    std::cout << "Waiting for connections..." << std::endl;
    
    while (true) {
        sockaddr_in client_addr;
        int client_size = sizeof(client_addr);
        SOCKET clientfd = accept(sockfd, (sockaddr*)&client_addr, &client_size);
        
        if (clientfd == INVALID_SOCKET) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }
        
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(clientfd);
        }
        
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        std::cout << "New client connected: " << client_ip << std::endl;
        std::cout << "Total clients: " << clients.size() << std::endl;
        
        // 为新客户端创建线程
        std::thread client_thread(handle_client, clientfd);
        client_thread.detach();
    }
    
    closesocket(sockfd);
    WSACleanup();
    return 0;
}