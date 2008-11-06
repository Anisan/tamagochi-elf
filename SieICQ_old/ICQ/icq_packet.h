#ifndef _PACKET_H
#define _PACKET_H


//#include "icq2_module.h"

typedef struct {
	int size;
	int offset;
	void *data;
} Packet;

/* prototypes */
void icq_packet_free(Packet *packet);
void icq_packet_add(Packet *packet, void *data, int size);
int icq_packet_get(Packet *packet, void *data, int datalen);
void icq_packet_dump(Packet *packet);
void dump_data(unsigned char *data, int size);
/* !prototypes */

/* Appending functions */
#define PACKET_ADD_STR(packet, str) do { \
	char *__s = (str); \
	icq_packet_add(packet, __s, strlen(__s)); \
} while(0)

#define PACKET_ADD_8(packet, num) PACKET_ADD_TYPE(packet, num, guint8)
#define PACKET_ADD_16(packet, num) PACKET_ADD_TYPE(packet, htons(num), guint16)
#define PACKET_ADD_32(packet, num) PACKET_ADD_TYPE(packet, htonl(num), guint32)

#define PACKET_ADD_16LE(packet, num) \
   PACKET_ADD_TYPE(packet, GUINT16_TO_LE(num), guint16)

#define PACKET_ADD_32LE(packet, num) \
   PACKET_ADD_TYPE(packet, GUINT32_TO_LE(num), guint32)

#define PACKET_ADD_TYPE(packet, val, type) do { \
	type __val = (val); \
	icq_packet_add(packet, &__val, sizeof(type)); \
} while(0)

#define PACKET_ADD_PACKET(packet, add_packet) \
   icq_packet_add(packet, add_packet->data, add_packet->size)

#define PACKET_ADD_TLV(packet, type, size) do { \
	PACKET_ADD_16(packet, type); \
	PACKET_ADD_16(packet, size); \
} while(0)

#define PACKET_ADD_TLV_RAW(packet, type, size, data) do { \
	PACKET_ADD_TLV(packet, type, size); \
	icq_packet_add(packet, data, size); \
} while(0)

#define PACKET_ADD_TLV_STR(packet, type, str) do { \
	char *__s2 = (str); \
	PACKET_ADD_TLV(packet, type, strlen(__s2)); \
	PACKET_ADD_STR(packet, __s2); \
} while(0)

#define PACKET_ADD_TLV_16(packet, type, num) do { \
	PACKET_ADD_TLV(packet, type, 2); \
	PACKET_ADD_16(packet, num); \
} while(0)

#define PACKET_ADD_TLV_32(packet, type, num) do { \
	PACKET_ADD_TLV(packet, type, 4); \
	PACKET_ADD_32(packet, num); \
} while(0)

/* Retrieval functions */
#define PACKET_GET_TYPE(packet, val, type) do { \
	if (icq_packet_get(packet, &val, sizeof(type)) != sizeof(type)) \
		n_error("Not enough data left in packet"); \
} while(0)

#define PACKET_GET_8(packet, num) PACKET_GET_TYPE(packet, num, guint8)

#define PACKET_GET_16(packet, num) do { \
	PACKET_GET_TYPE(packet, num, guint16); \
	num = ntohs(num); \
} while(0)

#define PACKET_GET_32(packet, num) do { \
	PACKET_GET_TYPE(packet, num, guint32); \
	num = ntohl(num); \
} while(0)

#define PACKET_GET_16LE(packet, num) do { \
	PACKET_GET_TYPE(packet, num, guint16); \
	num = GUINT16_FROM_LE(num); \
} while(0)

#define PACKET_GET_32LE(packet, num) do { \
	PACKET_GET_TYPE(packet, num, guint32); \
	num = GUINT32_FROM_LE(num); \
} while(0)

#define PACKET_GET_STR(packet, str, len) do { \
	int __len = (len); \
	str = d_malloc(__len + 1); \
	icq_packet_get(packet, str, __len); \
	str[__len] = 0; \
} while(0)

#define PACKET_GET_STR0(packet, str, len) do { \
	int __len = (len); \
	str = d_malloc(__len); \
	icq_packet_get(packet, str, __len); \
} while(0)

#define PACKET_GET_LEN_STR0(packet, str) do { \
	guint16 _slen; \
	PACKET_GET_16LE(packet, _slen); \
	PACKET_GET_STR0(packet, str, _slen); \
} while (0)

#define PACKET_GET_TLV(packet, type, length) do { \
	PACKET_GET_16(packet, type); \
	PACKET_GET_16(packet, length); \
} while(0)

#define PACKET_SKIP(packet, length) (packet)->offset += length

#define PACKET_SKIP_TLVS(packet, tlvs) do { \
	guint16 __len, __num_tlvs = (tlvs); \
	while (__num_tlvs-- > 0) { \
		PACKET_SKIP(packet, 2); \
		PACKET_GET_16(packet, __len); \
		PACKET_SKIP(packet, __len); \
	} \
} while(0)

#endif /* _PACKET_H */

 
 
