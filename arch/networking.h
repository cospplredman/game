
#ifndef GAME_NETWORKING_H
#define GAME_NETWORKING_H

//TODO

void send_to_peer(int peer, char* buf, size_t len);
void connect_to_peer(char *address, int port);
int setup_peer_con(int port, void (*fn)(int, char*, size_t));
int get_max_peers();
int get_peers();
void disconnect_peer(int);

#endif

