#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>


//TODO deal with errors properly :/

void dummy_callback(int peer, char* buf, size_t len){
	buf[len] = 0;
	printf("peer %d: %s\n", peer, buf);
}

static void (*packet_callback)(int peer, char *buf, size_t len) = dummy_callback;



static struct peer_con {
	int send;
	int recv;
	struct sockaddr addr;
} peer[10];

static int peers = 0;

void send_to_peer(int p, char *buf, size_t len){
	send(peer[p].send, buf, len, 0);
}

int compare_addresses(const struct sockaddr *addr1, const struct sockaddr *addr2) {
    // Check if the address families are the same
    if (addr1->sa_family != addr2->sa_family) {
        return 0; // Address families are different
    }

    // Compare the addresses based on the address family
    if (addr1->sa_family == AF_INET) {
        // IPv4 address comparison
        const struct sockaddr_in *ipv4_addr1 = (const struct sockaddr_in *)addr1;
        const struct sockaddr_in *ipv4_addr2 = (const struct sockaddr_in *)addr2;
        return ipv4_addr1->sin_addr.s_addr == ipv4_addr2->sin_addr.s_addr;
    }

    // Unsupported address family
    return 0;
}

void print_sockaddr(const struct sockaddr *addr) {
    char ip_str[INET6_ADDRSTRLEN];

    if (addr->sa_family == AF_INET) {
        const struct sockaddr_in *ipv4_addr = (const struct sockaddr_in *)addr;
        inet_ntop(AF_INET, &(ipv4_addr->sin_addr), ip_str, INET_ADDRSTRLEN);
        printf("IPv4 Address: %s, Port: %d\n", ip_str, ntohs(ipv4_addr->sin_port));
    } else if (addr->sa_family == AF_INET6) {
        const struct sockaddr_in6 *ipv6_addr = (const struct sockaddr_in6 *)addr;
        inet_ntop(AF_INET6, &(ipv6_addr->sin6_addr), ip_str, INET6_ADDRSTRLEN);
        printf("IPv6 Address: %s, Port: %d\n", ip_str, ntohs(ipv6_addr->sin6_port));
    } else {
        printf("Unsupported address family: %d\n", addr->sa_family);
    }
}


void connect_to_peer(char *address, int port){
	if(peers >= 10)
		return;

	struct sockaddr_in serv_addr;
	int client_fd;
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		//printf("\n Socket creation error \n");
		return;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if (inet_pton(AF_INET, address, &serv_addr.sin_addr) <= 0) {
		//printf("\nInvalid address/ Address not supported \n");
		return;
	}

	if (connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		//printf("\nConnection Failed \n");
		return;
	}

	//printf("[*] connected to %s:%d", address, port);

	struct sockaddr addr;
	socklen_t sz = sizeof(addr);
	getsockname(client_fd, &addr, &sz);

	for(int i = 0; i < peers; i++){
		if(compare_addresses(&peer[i].addr, &addr)){
			peer[i].send = client_fd;
			return;
		}
	}

	printf("connecting to\n");
	print_sockaddr(&addr);
	peer[peers++] = (struct peer_con){.send = client_fd, .recv = -1, .addr = addr};
}

void handle_packets(int p) {
	char buffer[1024];
	int bytes_received;

	while(1){
		bytes_received = recv(peer[p].recv, buffer, sizeof(buffer), 0);
		if (bytes_received <= 0)
			break;

		packet_callback(p, buffer, bytes_received);
	}
}

void *handle_peers(void *cb){
	int server_socket = *((int*)cb);
	while(1){
		{
			struct sockaddr addr;
			socklen_t sz = sizeof(addr);
			int fd = accept(server_socket, &addr, &sz);
			if(fd >= 0){
				for(int i = 0; i < peers; i++){
					if(compare_addresses(&peer[i].addr, &addr)){
						peer[i].recv = fd;
						goto skip;
					}
				}
				
				printf("new connection from:\n");
				print_sockaddr(&addr);
				
				peer[peers++] = (struct peer_con){.send = -1, .recv = fd, .addr = addr};

			skip:;
			}
		}

		for(int i = 0; i < peers; i++)
			handle_packets(i);
	}
}

int setup_listener_sock(int PORT){
	int server_socket;
	struct sockaddr_in server_address;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		perror("Error creating socket");
		return -1;
	}

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);

	if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
		perror("Error binding socket");
		return -1;
	}

	if (listen(server_socket, 5) == -1) {
		perror("Error listening for connections");
		return -1;
	}

	int flags;
	if ((flags = fcntl(server_socket, F_GETFL, 0)) == -1) {
		perror("fcntl F_GETFL");
		return -1;
	}

	if (fcntl(server_socket, F_SETFL, flags | O_NONBLOCK) == -1) {
		perror("fcntl F_SETFL");
		return -1;
	}

	//printf("[*] Server listening on port %d\n", PORT);
	return server_socket;
}

int get_max_peers(){
	return 10;
}

int setup_peer_con(int port, void (*fn)(int, char*, size_t)){
	packet_callback = fn;
	int server_socket = setup_listener_sock(port);

	if(server_socket < 0)
		return -1;

	pthread_t network_thread;
	if (pthread_create(&network_thread, NULL, handle_peers, &server_socket) != 0) {
		perror("Error creating thread");
	}
	pthread_detach(network_thread);

	return 0;
}

/*
int main(int c, char **argv) {
	if(c != 2){
		//printf("usage: p2p PORT\n");
		return 1;
	}

	int PORT = atoi(argv[1]);
	int server_socket = setup_listener_sock(PORT);

	//char buf[100];
	char *buf = "127.1.1.1";
	int peer_port;
	//printf("address: ");
	//scanf("%s", buf);
	printf("port: ");
	scanf("%d", &peer_port);

	printf("got: %s:%d\n", buf, peer_port);

	pthread_t network_thread;
	if (pthread_create(&network_thread, NULL, handle_peers, &server_socket) != 0) {
		perror("Error creating thread");
	}
	pthread_detach(network_thread);

	connect_to_peer(buf, peer_port);

	while(1){
		char buf[100];
		scanf("%s", buf);
		send(peer[0].send, buf, strlen(buf), 0);
	}
	
	return 0;
}
*/
