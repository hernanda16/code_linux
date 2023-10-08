//--Essential
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

//--Network
#include <arpa/inet.h>
#include <sys/socket.h>

#define PC_RECV_PORT 8888
#define STM_RECV_PORT 44444
#define RECV
#define SEND

/**
 * @note socket - return a file descriptor for the pc new socket
 * @note bind - give socket FD the local address ADDR (which is LEN bytes long)
*/
int socket_;
struct sockaddr_in server; //STM Address (Need to configure to send)
struct sockaddr_in any_addr; //Any Address sending to this PC
socklen_t any_addr_len = sizeof(any_addr); //Lenght Any Address
struct sockaddr_in client; //PC Address (Need to configure to recv)

char recv_buffer[32];
char send_buffer[32] = "its";

int main()
{
    //Open socket and checking is socket opened properly
    socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_ < 0) {
        printf("Socket Error\n");
        return -1;
    }

#ifdef SEND
    //Clean data and set STM configuration
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(STM_RECV_PORT);
    server.sin_addr.s_addr = inet_addr("10.0.3.207");
#endif

#ifdef RECV
    //Clean data and set PC configuration
    bzero(&client, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(PC_RECV_PORT);
    client.sin_addr.s_addr = INADDR_ANY;

    //Bind socket and checking is binding success
    bind(socket_, (struct sockaddr*)&client, sizeof(client));
    if (socket_ < 0) {
        printf("Binding Failed\n");
        return -1;
    }
#endif

    while (1) {
#ifdef RECV
        int8_t recv_len = recvfrom(socket_, recv_buffer, 64, MSG_DONTWAIT, (struct sockaddr*)&any_addr, &any_addr_len);
        if (recv_len > 0 && recv_buffer[0] == 'i' && recv_buffer[1] == 't' && recv_buffer[2] == 's') {
            for (int8_t i = 0; i < 3; i++) {
                printf("recv_buffer[%d] = %c \n", i, recv_buffer[i]);
            }
            printf("recv_buffer[3] = %d \n\n", recv_buffer[3]);
        }
#endif

#ifdef SEND
        static int8_t n;
        memcpy(send_buffer + 3, &n, 1);
        n++;
        sleep(1);
        sendto(socket_, send_buffer, sizeof(send_buffer), 0, (struct sockaddr*)&server, sizeof(struct sockaddr_in));
#endif
    }
}