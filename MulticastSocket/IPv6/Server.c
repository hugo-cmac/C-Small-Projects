#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



int multicastSocket(int family, int recvport, char *multiCastAddr, char *interface, int loopback, int ttl, int recvBuffSize, int sendBuffSize){
    
    if (family == AF_INET || family == AF_INET6){
        
        //////////////
        // Open Socket
        int s = socket(family, SOCK_DGRAM, 0);
        if (s < 0){
            perror("Error on openning Socket descriptor");
            return -1;
        }

        ////////////////////////
        //Socket binding to port
        struct sockaddr_in6 addr = {family, htons(recvport)};
        if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0){
            perror("Error on port binding");
            return -1;
        }

        if (family == AF_INET){

            /////////////////////
            //Add Multicast group
            struct ip_mreq grp = {0};
            inet_pton(AF_INET, multiCastAddr, &grp.imr_multiaddr);
            if(setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &grp, sizeof(grp)) < 0){
                perror("Error on adding group");
                return -1;
            }
            
            //////////////
            //Set loopback
            if(setsockopt(s, IPPROTO_IP , IP_MULTICAST_LOOP, &loopback, sizeof(loopback)) < 0){
                perror("Error on loopback setting");
                return -1;
            }

            ////////////////////////
            //Set time to live(hops)
            if(setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0){
                perror("Error on ttl setting");
                return -1;
            }
        }else{
            
            /////////////////////
            //Add Multicast group
            struct ipv6_mreq grp = {0};
            inet_pton(AF_INET6, multiCastAddr, &grp.ipv6mr_multiaddr);
            if(setsockopt(s, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &grp, sizeof(grp)) < 0){
                perror("Error on adding group");
                return -1;
            }

            //////////////
            //Set loopback
            if(setsockopt(s, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &loopback, sizeof(loopback)) < 0){    
                perror("Error on loopback setting");
                return -1;
            }

            ////////////////////////
            //Set time to live(hops)
            if(setsockopt(s, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &ttl, sizeof(ttl)) < 0){    
                perror("Error on ttl setting");
                return -1;
            }
        }

        //////////////////////
        //Set send buffer size
        if(setsockopt(s, SOL_SOCKET, SO_SNDBUF, &sendBuffSize, 4) < 0){
            perror("Error on setting send buffer size");
            return -1;
        }

        //////////////////////
        //Set recv buffer size
        if(setsockopt(s, SOL_SOCKET, SO_RCVBUF, &recvBuffSize, 4) < 0){
            perror("Error on setting recv buffer size");
            return -1;
        }

        ///////////////////////
        //Set interface to bind
        if(setsockopt(s, SOL_SOCKET, SO_BINDTODEVICE, interface, 4) < 0){
            perror("Error on interface binding");
            return -1;
        }

        return s;
    }
    perror("Invalid family");
    return -1;
}

int main(int argc, char const *argv[]){
   
    //////////
    // Address
    char *multicastaddr = "FF02::1";

    ///////////////////
    // Multicast socket
    int mcSocket = multicastSocket(AF_INET6, 9999, multicastaddr, "eth0", 0, 1, 1024, 1024);
    if(mcSocket < 0){
        perror("Error opening multicast socket");
        return -1;
    }
        
    ////////////
    // Dest addr 
    struct sockaddr_in6 addr = {0};
    addr.sin6_family = AF_INET6;     addr.sin6_port = htons(8888);
    inet_pton(AF_INET6, multicastaddr, &addr.sin6_addr);

    char buffer[512] = {0};

    recv(mcSocket, buffer, 512, 0);
    printf("Client: %s\n", buffer);
    
    
    printf("Server: ");
    fgets(buffer, 512, stdin);
    sendto(mcSocket, buffer, 512, 0, (struct sockaddr*)&addr, sizeof(struct sockaddr_in6));
            
    close(mcSocket);
       
    return 0;
}