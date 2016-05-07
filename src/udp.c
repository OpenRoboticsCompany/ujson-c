/*
 * Copyright (c) 2016 Aaron Kondziela <aaron@aaronkondziela.com> 
 *
 * This file is part of ujson-c
 *
 * ujson-c is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ujson-c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */
 
 /*
  * Part of ujson-c - Implements microjson in C - see ujson.org
  * and https://github.com/aaronkondziela/ujson-c/
  *
  * udp.c
  *
  * Sends UDP packets. Used during development.
  *
  */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "udptarget.h"
#ifndef DEST_IP
#warning "DEST_IP not defined in udptarget.h - using default"
#define DEST_IP   "127.0.0.1"
#endif
#ifndef DEST_PORT
#warning "DEST_PORT not defined in udptarget.h - using default"
#define DEST_PORT 7777
#endif


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
