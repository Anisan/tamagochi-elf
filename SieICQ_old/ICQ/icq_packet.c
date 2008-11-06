//#include "icq2_module.h"

#include "icq_packet.h"

void icq_packet_free(Packet *packet) {
	mfree(packet->data);
	mfree(packet);
}

void icq_packet_add(Packet *packet, void *data, int size) {
	/* Make the space for the new data */
	packet->data = d_realloc(packet->data, packet->size + size);
	
	/* Copy the new data in */
	memcpy(packet->data + packet->size, data, size);
	
	/* Recalculate the size */
	packet->size += size;
}

int icq_packet_get(Packet *packet, void *data, int datalen) {
	if (!packet) return 0;
	
	if (packet->size - packet->offset < datalen) return 0;
	
	memcpy(data, packet->data + packet->offset, datalen);
	
	packet->offset += datalen;
	
	return datalen;
}

void icq_send_packet(ICQAccount *account, int channel, Packet *packet) {
	FLAPHeader header;
	char *packet_stream;
	
	if (!account || !packet) return;
	
	header.start = 0x2a;
	header.channel = channel;
	header.seq_num = htons(account->flap_seqno);
	header.length = htons(packet->size);
	
	/* Sequence number rolls over at 65535 */
	if (++account->flap_seqno > 0xffff) account->flap_seqno = 0;
	
	packet_stream = d_malloc(sizeof(FLAPHeader) + packet->size);
	memcpy(packet_stream, &header, sizeof(FLAPHeader));
	memcpy(packet_stream + sizeof(FLAPHeader), packet->data, packet->size);
	
	send(account->sock, packet_stream, sizeof(FLAPHeader) + packet->size, 0);
	
	/*
	 n_debug("Sent packet:");
	 icq_packet_dump(packet);
	 */
	
	d_free(packet_stream); 
	icq_packet_free(packet);
}

void icq_packet_dump(Packet *packet) {
	dump_data(packet->data, packet->size);
}

void dump_data(unsigned char *data, int size) {
	int i;
	
	fprintf(stderr, "Dumping Data:\n");
	
	for (i = 0; i < size; i++) {
		if (i % 16 == 0) fprintf(stderr, "\n");
		fprintf(stderr, "%02x ", data[i]);
	}
	
	fprintf(stderr, "\n\n");
}

 
 
