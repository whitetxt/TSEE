// Sending packets

// Packet types
typedef struct {
	char name[32];
} PACKET_CONNECT;

typedef struct {
	uint16_t x;
	uint16_t y;
} PACKET_MOVE;

typedef struct {
	char msg[512];
} PACKET_CHAT;

typedef struct {
	char name[32];
} PACKET_NAME_CHANGE;

typedef struct {
	uint16_t state;
} PACKET_CROUCH;

typedef struct {
	uint16_t challengeResult;
} PACKET_JOINING;

typedef struct UDPPacket {
	uint16_t packetNum;
	uint16_t type;
	union {
		PACKET_CONNECT connect;
		PACKET_MOVE move;
		PACKET_CHAT chat;
		PACKET_NAME_CHANGE nameChange;
		PACKET_CROUCH crouch;
		PACKET_JOINING join;
	};
} __attribute__((packed)) UDPPacket;

// Recieving packets

// Packet types
typedef struct {
	uint16_t ID;
	char name[32];
	uint16_t x;
	uint16_t y;
	uint16_t crouch;
} PACKET_CONNECT_RECV;

typedef struct {
	uint16_t ID;
	uint16_t x;
	uint16_t y;
} PACKET_MOVE_RECV;

typedef struct {
	uint16_t ID;
	char msg[512];
} PACKET_CHAT_RECV;

typedef struct {
	uint16_t ID;
	char name[32];
} PACKET_NAME_CHANGE_RECV;

typedef struct {
	uint16_t ID;
	uint16_t state;
} PACKET_CROUCH_RECV;

typedef struct {
	char challengeChars[5];
} PACKET_JOINING_RECV;

typedef struct UDPPacketRecv {
	uint16_t packetNum;
	uint16_t type;
	union {
		PACKET_CONNECT_RECV connect;
		PACKET_MOVE_RECV move;
		PACKET_CHAT_RECV chat;
		PACKET_NAME_CHANGE_RECV nameChange;
		PACKET_CROUCH_RECV crouch;
		PACKET_JOINING_RECV join;
	};
} __attribute__((packed)) UDPPacketRecv;

typedef enum UDPPacketType {
	PACKET_TYPE_CONNECT = 0,
	PACKET_TYPE_MOVE = 1,
	PACKET_TYPE_CHAT = 2,
	PACKET_TYPE_NAME_CHANGE = 3,
	PACKET_TYPE_CROUCH = 4,
	PACKET_TYPE_JOIN = 5
} UDPPacketType;

// TCP Sending Packets
typedef struct {
	uint16_t challengeResult;
	char name[32];
} PACKET_CHALLENGE;

typedef struct {
	uint16_t howAreYou;
} PACKET_PING;

typedef struct {
	char leaveMsg[32];
} PACKET_DISCONNECT;

typedef struct TCPPacket {
	uint16_t type;
	union {
		PACKET_CHALLENGE challenge;
		PACKET_PING ping;
		PACKET_DISCONNECT disconnect;
	};
} __attribute__((packed)) TCPPacket;

// TCP Recieving Packets
typedef struct {
	char challengeChars[6];
} PACKET_CHALLENGE_RECV;

typedef struct {
	uint16_t ID;
	uint16_t x;
	uint16_t y;
} PACKET_CONNECTED_RECV;

typedef struct {
	uint16_t imGood;
} PACKET_PING_RECV;

typedef struct {
	uint16_t ID;
	char leaveMsg[32];
} PACKET_DISCONNECT_RECV;

typedef struct TCPPacketRecv {
	uint16_t type;
	union {
		PACKET_CHALLENGE_RECV challenge;
		PACKET_CONNECTED_RECV connected;
		PACKET_PING_RECV ping;
		PACKET_DISCONNECT_RECV disconnect;
	};
} __attribute__((packed)) TCPPacketRecv;

typedef enum TCPPacketType {
	PACKET_TYPE_CHALLENGE = 0,
	PACKET_TYPE_PING = 1,
	PACKET_TYPE_CONNECTED = 2,
	PACKET_TYPE_DISCONNECT = 3
} TCPPacketType;

// Basic socket functions
int setupClient();
int sendMsgUDP(void *packetData, UDPPacketType type);

// Sending Packet Functions
void movePacket();
void disconnectPacket(char *reason);
void nameChangePacket(char *newName);
void crouchPacket(bool crouching);
void connectToServer();

// Receiving Packet Functions
int getUpdates();
int getUpdatesTCP();
void updateMultiplayerNames();