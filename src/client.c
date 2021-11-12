#include "include/main.h"
#include <zed_net.h>
#include <sys/time.h>

zed_net_address_t server;
zed_net_socket_t UDPSock;
zed_net_socket_t TCPSock;
int lastSentPos[2] = {0, 0};
uint16_t lastPacketNum = 0;
struct timeval pingStartTime;

// Sets up the multiplayer client.
// @return 0 on success, -1 on failure.
int setupClient() {
	if (zed_net_init() != 0) {
		printf("Error initialising zed_net: %s\n", zed_net_get_error());
		return -1;
	}

	zed_net_socket_t intSock;

	int rc = zed_net_udp_socket_open(&intSock, 0, 0); // Open the socket

	if (rc == -1) { // Check the socket opened
		printf("Error opening socket: %s\n", zed_net_get_error());
		zed_net_shutdown();
		return -1;
	}

	UDPSock = intSock;

	int hostIdx = atoi(strtok(NULL, ","));
	int portIdx = atoi(strtok(NULL, ","));
	char *host = menu->items[hostIdx].text;
	int port = atoi(menu->items[portIdx].text);

	if (zed_net_get_address(&server, host, port) != 0) { // Get the server's address
		printf("Error getting the server address: %s\n", zed_net_get_error());

		zed_net_socket_close(&UDPSock);
		zed_net_socket_close(&intSock);
		zed_net_shutdown();

		return -1;
	}

	printf("Host: %s\nPort: %d\n", host, port);

	rc = zed_net_tcp_socket_open(&TCPSock, 0, 0, 0); // Open the socket
	if (rc == -1) { // Check the socket opened
		printf("Error opening socket: %s\n", zed_net_get_error());
		zed_net_socket_close(&UDPSock);
		zed_net_socket_close(&intSock);
		zed_net_shutdown();
		return -1;
	}

	printf("Client setup.\n");

	return 0;
}

// Sends a message to the server via UDP.
// @param packetData The packet to send
// @param type The type of packet to send.
// @return 0 on success, -1 on failure.
int sendMsgUDP(void *packetData, UDPPacketType type) {
	UDPPacket *pack = malloc(sizeof(*pack));
	pack->packetNum = htons(lastPacketNum + 1);
	pack->type = htons(type);
	switch (type) {
		case PACKET_TYPE_CONNECT:
			pack->connect = *(PACKET_CONNECT *)packetData;
			break;
		case PACKET_TYPE_MOVE:
			pack->move = *(PACKET_MOVE *)packetData;
			break;
		case PACKET_TYPE_CHAT:
			pack->chat = *(PACKET_CHAT *)packetData;
			break;
		case PACKET_TYPE_NAME_CHANGE:
			pack->nameChange = *(PACKET_NAME_CHANGE *)packetData;
			break;
		case PACKET_TYPE_CROUCH:
			pack->crouch = *(PACKET_CROUCH *)packetData;
			break;
		default:
			printf("Error: Invalid packet type.\n");
			return -1;
	}
	if (zed_net_udp_socket_send(&UDPSock, server, pack, sizeof(*pack)) != 0) { // Check it sent correctly
		printf("Send failed: %s\n", zed_net_get_error());
		return -1;
	}
	lastPacketNum += 1;
	return 0;
}

// Sends a message to the server via TCP.
// @param packetData The packet to send
// @param type The type of packet to send.
// @return 0 on success, -1 on failure.
int sendMsgTCP(void *packetData, TCPPacketType type) {
	TCPPacket *pack = malloc(sizeof(*pack));
	pack->type = htons(type);
	switch (type) {
		case PACKET_TYPE_CHALLENGE:
			pack->challenge = *(PACKET_CHALLENGE *)packetData;
			break;
		case PACKET_TYPE_PING:
			pack->ping = *(PACKET_PING *)packetData;
			break;
		case PACKET_TYPE_DISCONNECT:
			pack->disconnect = *(PACKET_DISCONNECT *)packetData;
			break;
		default:
			printf("Unknown packet type.\n");
			return -1;
	}
	if (zed_net_tcp_socket_send(&TCPSock, pack, sizeof(*pack)) != 0) { // Check it sent correctly
		printf("Send failed: %s\n", zed_net_get_error());
		return -1;
	}
	return 0;
}

// Sends the player's position to the server.
void movePacket() {
	PACKET_MOVE pack;
	pack.x = MainPlayer.rect.x + worldScrollX;
	pack.y = MainPlayer.rect.y;
	if (pack.x == lastSentPos[0] && pack.y == lastSentPos[1]) {
		return;
	}
	if (sendMsgUDP(&pack, PACKET_TYPE_MOVE) != 0) {
		printf("Failed to send.\n");
	}
	lastSentPos[0] = pack.x;
	lastSentPos[1] = pack.y;
}

// Creates and sends the disconnect packet.
// @param reason The reason for disconnecting.
void disconnectPacket(char *reason) {
	PACKET_DISCONNECT pack;
	sprintf(pack.leaveMsg, "%s", reason);
	if (sendMsgTCP(&pack, PACKET_TYPE_DISCONNECT) != 0) {
		printf("Failed to send.\n");
	}
}

// Creates and sends the name change packet.
// @param newName The new name to change to.
void nameChangePacket(char *newName) {
	PACKET_NAME_CHANGE pack;
	sprintf(pack.name, "%s", newName);
	if (sendMsgUDP(&pack, PACKET_TYPE_NAME_CHANGE) != 0) {
		printf("Failed to send.\n");
	}
}

// Creates and sends the crouch packet.
// @param crouching Whether or not the player is crouching.
void crouchPacket(bool crouching) {
	PACKET_CROUCH pack;
	pack.state = crouching;
	if (sendMsgUDP(&pack, PACKET_TYPE_CROUCH) != 0) {
		printf("Failed to send.\n");
	}
}

// Sends a ping packet to the server.
void pingPacket() {
	PACKET_PING pack;
	if (sendMsgTCP(&pack, PACKET_TYPE_PING) != 0) {
		printf("Failed to send.\n");
	} else {
		gettimeofday(&pingStartTime, NULL);
	}
}

// Performs the routine to connect to the server.
void connectToServer() {
	if (zed_net_tcp_connect(&TCPSock, server)) {
		printf("Failed to connect: %s\n", zed_net_get_error());
		return;
	}
	printf("Connected to server! Doing challenge...\n");
	TCPPacketRecv *pack = malloc(sizeof(*pack));
	int bytes = zed_net_tcp_socket_receive(&TCPSock, pack, sizeof(*pack));
	if (bytes == 0) {
		printf("Failed to receive challenge: %s\n", zed_net_get_error());
		return;
	}
	pack->type = ntohs(pack->type);
	if (pack->type != PACKET_TYPE_CHALLENGE) {
		printf("Invalid challenge packet.\n");
		return;
	}
	printf("Recieved challenge: %s\n", pack->challenge.challengeChars);
	uint16_t challengeResult = 0;
	for (int i = 0; i < 5; i++) {
		challengeResult += ((pack->challenge.challengeChars[i] * 6) * i) - (2 << i);
	}
	printf("%d\n", challengeResult);
	PACKET_CHALLENGE *chalPack = malloc(sizeof(*chalPack));
	chalPack->challengeResult = htons(challengeResult);
	sprintf(chalPack->name, "%s", Settings.name);
	if (sendMsgTCP(chalPack, PACKET_TYPE_CHALLENGE) != 0) {
		printf("Failed to send challenge.\n");
		return;
	}
	free(chalPack);
	bytes = zed_net_tcp_socket_receive(&TCPSock, pack, sizeof(*pack));
	if (bytes == 0) {
		printf("Failed to recieve connected packet.\n");
		return;
	}
	pack->type = ntohs(pack->type);
	if (pack->type != PACKET_TYPE_CONNECTED) {
		printf("Invalid connected packet.\n");
		return;
	}
	printf("Connected to server.\n");
	getUpdate = SDL_CreateThread(getUpdates, "updateThread", NULL);
	if (!getUpdate) {
		printf("Failed to start the update thread: %s\n", SDL_GetError());
		return;
	}
	SDL_Delay(100);
	UDPPacket *connPack = malloc(sizeof(*connPack));
	connPack->type = htons(PACKET_TYPE_CONNECT);
	sprintf(connPack->connect.name, "%s", Settings.name);
	if (sendMsgUDP(connPack, PACKET_TYPE_CONNECT) != 0) {
		printf("Failed to send connect packet.\n");
		return;
	}
	return;
}

// Gets updates from the server.
// @return 0 on success, -1 on failure.
int getUpdates() {
	// !THIS NEEDS UPDATING TO USE THE NEW PACKET SYSTEM
	while (playing) {
		UDPPacketRecv *packet = malloc(sizeof(*packet));
		int bytes = zed_net_udp_socket_receive(&UDPSock, &server, packet, sizeof(*packet));
		if (bytes == -1) {
			printf("Failed to get updates: %s\n", zed_net_get_error());
			return -1;
		}
		printf("Recieved %d bytes packet.\n", bytes);
		char msg[128];
		if (!packet) {
			continue;
		}
		switch (packet->type) {
			case PACKET_TYPE_CONNECT:
				printf("Player has joined the server: %s\n", packet->connect.name);
				sprintf(msg, "%s has joined!", packet->connect.name);
				createToast(msg);
				players = realloc(players, ++numPlayers * sizeof(*players));
				struct OtherPlayer tmpPlayer;
				tmpPlayer.worldRect.x = packet->connect.x;
				tmpPlayer.worldRect.y = packet->connect.y;
				SDL_QueryTexture(playerTexture, NULL, NULL, &tmpPlayer.rect.w, &tmpPlayer.rect.h);
				tmpPlayer.rect.x = tmpPlayer.worldRect.x - worldScrollX;
				tmpPlayer.rect.y = tmpPlayer.worldRect.y;
				tmpPlayer.ID = packet->connect.ID;
				tmpPlayer.name = strdup(packet->connect.name);
				tmpPlayer.nameTag = createText(tmpPlayer.name, tmpPlayer.rect.x + tmpPlayer.rect.w / 2, tmpPlayer.rect.y + 10, playerFont);
				tmpPlayer.nameTag.rect.y += tmpPlayer.nameTag.rect.h;
				players[numPlayers - 1] = tmpPlayer;
				break;
			case PACKET_TYPE_MOVE:
				printf("Player moved to %d:%d\n", packet->move.x, packet->move.y);
				for (size_t x = 0; x < numPlayers; x++) {
					if (packet->move.ID == players[x].ID) {
						players[x].worldRect.x = packet->move.x;
						players[x].worldRect.y = packet->move.y;
						players[x].rect.x = packet->move.x - worldScrollX;
						players[x].rect.y = packet->move.y;
					}
				}
				break;
			case PACKET_TYPE_CHAT:
				printf("Player sent: %s\n", packet->chat.msg);
				break;
			case PACKET_TYPE_NAME_CHANGE:
				printf("Player changed name to: %s\n", packet->nameChange.name);
				for (size_t x = 0; x < numPlayers; x++) {
					if (packet->nameChange.ID == players[x].ID) {
						players[x].name = strdup(packet->nameChange.name);
						players[x].nameTag = createText(players[x].name, players[x].rect.x + players[x].rect.w / 2, players[x].rect.y + 10, playerFont);
						players[x].nameTag.rect.y += players[x].nameTag.rect.h;
					}
				}
				break;
			case PACKET_TYPE_CROUCH:
				packet->crouch.state = ntohs(packet->crouch.state);
				printf("Player changed to crouching state: %d\n", packet->crouch.state);
				for (size_t x = 0; x < numPlayers; x++) {
					if (packet->crouch.ID == players[x].ID) {
						players[x].crouching = packet->crouch.state;
					}
				}
				break;
			default:
				printf("Unknown packet type recieved: %d\n", packet->type);
		}
		free(packet);
	}
	return 0;
}

int getUpdatesTCP() {
	while (playing) {
		TCPPacketRecv *packet = NULL;
		int bytes = zed_net_tcp_socket_receive(&TCPSock, packet, sizeof(*packet));
		if (bytes == -1) {
			printf("Failed to get updates: %s\n", zed_net_get_error());
			return -1;
		}
		if (!packet) {
			continue;
		}
		struct timeval current_time;
		switch (packet->type) {
			case PACKET_TYPE_DISCONNECT:
				for (size_t x = 0; x < numPlayers; x++) {
					if (packet->disconnect.ID == players[x].ID) {
						for (size_t y = x; y < numPlayers; y++) {
							players[y] = players[y + 1];
						}
						numPlayers--;
						break;
					}
				}
				break;
			case PACKET_TYPE_PING:
				gettimeofday(&current_time, NULL);
				long int secsDiff = current_time.tv_sec - pingStartTime.tv_sec;
				long int usecsDiff = current_time.tv_usec - pingStartTime.tv_usec;
				long int pingTime = secsDiff * 1000 + usecsDiff / 1000;
				printf("Ping: %ldms\n", pingTime);
				break;
			default:
				printf("Unknown packet type recieved: %d\n", packet->type);
		}
	}
	return 0;
}

// Updates the positions and names of other players
void updateMultiplayerNames() {
	// Updates the positions and names of other players
	for (size_t x = 0; x < textArrSize; x++) {
		textArr[x].rect.x = (players[x].rect.x + (players[x].rect.w / 2)) - (textArr[x].rect.w / 2); // Move it on the X
		textArr[x].rect.y = players[x].rect.y - textArr[x].rect.h - 5; // Move it on the Y
	}
}