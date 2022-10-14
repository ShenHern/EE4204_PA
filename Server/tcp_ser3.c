/**********************************
tcp_ser.c: the source file of the server in tcp transmission 
***********************************/


#include "headsock.h"

#define BACKLOG 10

void str_ser(int sockfd);                                                        // transmitting and receiving function

int main(void)
{
    int sockfd, con_fd, ret;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int sin_size;

//	char *buf;
    pid_t pid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);          //create socket
    if (sockfd <0)
    {
        printf("error in socket!");
        exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYTCP_PORT);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr("172.0.0.1");
    bzero(&(my_addr.sin_zero), 8);
    ret = bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr));                //bind socket
    if (ret <0)
    {
        printf("error in binding");
        exit(1);
    }

    ret = listen(sockfd, BACKLOG);                              //listen
    if (ret <0) {
        printf("error in listening");
        exit(1);
    }

    while (1)
    {
        printf("waiting for data\n");
        sin_size = sizeof (struct sockaddr_in);
        con_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);            //accept the packet
        if (con_fd <0)
        {
            printf("error in accept\n");
            exit(1);
        }

        if ((pid = fork())==0)                                         // creat acception process
        {
            close(sockfd);
            str_ser(con_fd);                                          //receive packet and response
            close(con_fd);
            exit(0);
        }
        else close(con_fd);                                         //parent process
    }
    close(sockfd);
    exit(0);
}

void str_ser(int sockfd)
{
    char buf[BUFSIZE];
    FILE *fp;
    char recvs[DATALEN];
    struct ack_so ack;
    struct pack_so pack;
    int end, n = 0;
    long lseek=0;
    end = 0;
    int count=0;
    uint32_t prev_packnum = 15000;

    printf("receiving data!\n");

    while(!end)
    {
        if ((n= recv(sockfd, &pack, sizeof(struct pack_so), MSG_WAITALL))==-1)                                   //receive the packet
        {
            printf("error when receiving\n");
            exit(1);
        }
        count++;
//        pack.len = ntohl(pack.len);
//        pack.num = ntohl(pack.num);
        printf("%d. bytes_recv: %d, pack.len: %i, pack.num = %i\n", count, n, pack.len, pack.num);
        if (pack.data[pack.len-1] == '\0')									//if it is the end of the file
        {
            end = 1;
            pack.len --;
        }

        //send ACK after receiving N packets
        if (count % BATCHSIZE == 0){
            ack.len = 0;
            ack.num = pack.num + 1;
            n = send(sockfd, &ack, 2, 0);
            if(n==-1){
                printf("ERROR SENDING ACK!\n");
                exit(1);
            }
        }

        //write to file buf if not a previous packet
        if (pack.num == prev_packnum)
            continue;
        memcpy((buf+lseek), pack.data, pack.len);
        lseek += pack.len;
        prev_packnum = pack.num;
    }
    ack.num = 1;
    ack.len = 0;
    if ((n = send(sockfd, &ack, 2, 0))==-1)
    {
        printf("send error!");								//send the ack
        exit(1);
    }
    if ((fp = fopen ("myTCPreceive.txt","wt")) == NULL)
    {
        printf("File doesn't exit\n");
        exit(0);
    }
    fwrite (buf , 1 , lseek , fp);					//write data into file
    fclose(fp);
    printf("a file has been successfully received!\nthe total data received is %d bytes\n", (int)lseek);
}
