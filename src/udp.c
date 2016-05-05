#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>

#define DEST_IP   "192.168.2.175"
#define DEST_PORT 7777

// quick and dirty udp sender for testing

void xmit(uint8_t* buf, uint16_t len)
{
	int s;
	struct sockaddr_in addr;
	memset((char*)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEST_PORT);
	addr.sin_addr.s_addr = inet_addr(DEST_IP);
	s = socket(AF_INET, SOCK_DGRAM, 0);
	sendto(s, buf, len, 0, &addr, sizeof(addr));
	close(s);
}
