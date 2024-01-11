#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>


//TODO deal with errors properly :/
void dummy_callback(int peer, char* buf, size_t len){
	(void)peer;
	buf[len] = 0;
	//printf("peer %d: %s\n", peer, buf);
}

static void (*packet_callback)(int peer, char *buf, size_t len) = dummy_callback;

static int server_socket;
static struct peer_con {
	int sock;
	struct sockaddr addr;
} peer[10];
static int peers = 0;

void disconnect_peer(int p){
	close(peer[p].sock);
	for(int i = p+1; i < peers; i++)
		peer[i-1]=peer[i];

	peers--;
}

void connect_to_peer_(struct sockaddr *serv_addr){
	int client_fd;
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		//printf("\n Socket creation error \n");
		return;
	}


	if (connect(client_fd, serv_addr, sizeof(*serv_addr)) < 0) {
		//printf("\nConnection Failed \n");
		return;
	}

	struct sockaddr addr;
	socklen_t sz = sizeof(addr);
	getpeername(client_fd, &addr, &sz);
	//printf("[*] connection to peer\n");
	peer[peers++] = (struct peer_con){.sock = client_fd, .addr = addr};
}

void connect_to_peer(char *address, int port){
	struct sockaddr_in serv_addr;
	if(peers >= 10)
		return;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if (inet_pton(AF_INET, address, &serv_addr.sin_addr) <= 0) {
		//printf("\nInvalid address/ Address not supported \n");
		return;
	}

	connect_to_peer_((struct sockaddr*)&serv_addr);
}

void handle_packets(int p) {
	char buffer[1024];
	int bytes_received;

	while(1){
		bytes_received = recv(peer[p].sock, buffer, sizeof(buffer), 0);
		if (bytes_received == 0){
			//printf("disconnected from peer %d.\n", p);
			disconnect_peer(p);
		}

		if (bytes_received < 0)
			break;

		packet_callback(p, buffer, bytes_received);
	}
}
int setup_listener_sock(int PORT){
	int server_socket;
	struct sockaddr_in server_address;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		//printf("Error creating socket\n");
		return -1;
	}

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);

	if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
		//printf("Error binding socket\n");
		return -1;
	}

	if (listen(server_socket, 5) == -1) {
		//printf("Error listening for connections\n");
		return -1;
	}

	int flags;
	if ((flags = fcntl(server_socket, F_GETFL, 0)) == -1) {
		//printf("fcntl F_GETFL\n");
		return -1;
	}

	if (fcntl(server_socket, F_SETFL, flags | O_NONBLOCK) == -1) {
		//printf("fcntl F_SETFL\n");
		return -1;
	}

	//printf("0.0.0.0:%d server fd %d\n", PORT, server_socket);
	return server_socket;
}

int get_peers(){
	return peers;
}

int get_max_peers(){
	return 10;
}

void send_to_peer(int p, char *buf, size_t len){
	send(peer[p].sock, buf, len, 0);
}

void *handle_peers(void *cb){
	(void)cb;
	while(1){
		if (peers < 10) {
			struct sockaddr addr;
			socklen_t sz = sizeof(addr);
			int fd = accept(server_socket, &addr, &sz);
			//printf("%d\n", fd);
			if(fd >= 0){
				//printf("[*] connection to peer :)\n");
				peer[peers++] = (struct peer_con){.sock = fd, .addr = addr};
			}
		}

		for(int i = 0; i < peers; i++)
			handle_packets(i);
	}
}

int setup_peer_con(int port, void (*fn)(int, char*, size_t)){
	packet_callback = fn;
	server_socket = setup_listener_sock(port);

	if(server_socket < 0)
		return -1;

	pthread_t network_thread;
	if (pthread_create(&network_thread, NULL, handle_peers, NULL) != 0)
		return -1;

	pthread_detach(network_thread);
	return 0;
}
