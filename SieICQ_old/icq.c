#include "include.h"
#include "icq.h"
#include "main.h"
#include "icq_packet.h"
#include "Random.h"

//#include "icq_snac.h"



/* This was borrowed from libfaim */
char *icq_encode_password(char *password) {
	char encoding_table[] = {
			0xf3, 0x26, 0x81, 0xc4,
			0x39, 0x86, 0xdb, 0x92,
			0x71, 0xa3, 0xb9, 0xe6,
			0x53, 0x7a, 0x95, 0x7c
	};
	char *encoded = malloc(128);
        //sprintf(encoded,password);
	int i;
	
	for (i = 0; i < strlen(password); i++)
		encoded[i] = (password[i] ^ encoding_table[i]);
	
	return encoded;
}

/////////////////////////////////////////////
void _WriteLogICQ(char *buf, int size, int in_out)
{
//  return;
  int flog=-1;
  unsigned int err;
  flog = fopen("4:\\ZBin\\sieicq\\logs\\icq.log",A_ReadWrite + A_Create + A_Append + A_BIN,P_READ+P_WRITE,&err);
        if (flog!=-1)
	{
		char msg[4096];

		TTime t;
		TDate d;
		GetDateTime(&d,&t);
                if (in_out)
		sprintf(msg, " ->%02d:%02d:%02d ", t.hour,t.min,t.sec);
                else
		sprintf(msg, " <-%02d:%02d:%02d ", t.hour,t.min,t.sec);
		//  sprintf(msg, "%s\n", buf);
		fwrite(flog,msg,strlen(msg),&err);
		fwrite(flog,buf,size,&err);
	}
  fclose(flog,&err);      
}

   unsigned int UIN;
   char PASS[9];
   
   char *auth_cookie;
   int auth_length;
   int flap_seqno;
   int curr_reqid;
   short int icq_seqno;
   
   int login_bos;
   
   char * Host;
   unsigned int Port;
   short int ICQStatus;
   short int flags_status;
  
   int tenseconds_to_ping;
//////////////////////////////////////////
void CreateICQ()
{
 
  flags_status =0x0101;// INVISIBLE IP
 // flags_status =0x0000; // VISIBLE IP
 // Socket = new SocketAbstract();
 // Socket->Create();
  auth_cookie=NULL;
  curr_reqid = 0;
 // icq_seqno = 2;
  Randomize();
}

void CloseICQ()
{

}


void Login() 
{

  
  int ip=str2ip(Host);
  if (ip!=0xFFFFFFFF)
  {

//   Socket->Start(ip,Port);
  }
  else
  {

//   Socket->Start(Host,Port);
  }
  
  flap_seqno = rand() % 65063;
  curr_reqid = 0;
  icq_seqno = 2;
}



extern void Send(char* data,int size);

void send_packet(int channel,Packet* packet)
{
//          _WriteLog("head malloc");

        FLAP_HEAD header;// = malloc(sizeof(FLAP_HEAD));
	
	if (!packet) return;
	
        //_WriteLogICQ((char*)&header,sizeof(FLAP_HEAD));
        
	header.id = 0x2a;
	header.channel = channel;
	header.seq_number = htons(flap_seqno);
	header.data_size = htons(packet->size);
	
       // _WriteLogICQ((char*)&header,sizeof(FLAP_HEAD),1);

	/* Sequence number rolls over at 65535 */
	if (++flap_seqno > 0xffff) flap_seqno = 0;
	
	char *packet_stream;
	packet_stream = (char*)malloc(sizeof(FLAP_HEAD) + packet->size);
	memcpy(packet_stream, &header, sizeof(FLAP_HEAD));
	memcpy(packet_stream + sizeof(FLAP_HEAD), packet->data, packet->size);
	
        _WriteLogICQ(packet_stream, sizeof(FLAP_HEAD) + packet->size,0);
        
	Send(packet_stream, sizeof(FLAP_HEAD) + packet->size);
	
	mfree(packet_stream); 
	PackFree(packet);
        //mfree(header);
}

void send_message(Packet *packet) {
	Packet *full_packet;
	
	/* Create the actual packet that gets sent */
	full_packet = PackNew();
        snac_new(full_packet, 0x0015, 0x0002, NULL, NULL);
	
	/* Add TLV of contents */
	PackAddTLV(full_packet, 0x0001, packet->size + 2);
	
	/* The request length in little endian.  Isn't it great how ICQ
	 * wants to use OSCAR but still has to have it's little endian shit
	 * everywhere? */
	PackAdd16LE(full_packet, packet->size);
	
	/* Add what we had previously built */
	PackAddPack(full_packet, packet);
	
	send_packet(0x02, full_packet);
	PackFree(packet);
}


void send_login() 
{

        login_bos=0;
//        _WriteLog("new pack");
        Packet *packet=PackNew();
	
       /* Raw start */
	PackAdd32(packet, 0x00000001);
       
	/* ICQ Number (login) */
        char *tmp = malloc(32);
        sprintf(tmp,"%d",UIN);
//        _WriteLog("tlv str");
	PackAddTLVStr(packet, 0x0001, tmp);
        mfree(tmp);
        
	/* Encoded password */
	char *encoded = malloc(128);
	sprintf(encoded,icq_encode_password(PASS));
	PackAddTLVStr(packet, 0x0002, encoded);
	mfree(encoded);
	
	PackAddTLVStr(packet, 0x0003, ICQ_CLIENT_STRING);
	
	/* Unknown */

        PackAddTLV16(packet, 0x0016, 0x010a);
	
	/* Client version */
	PackAddTLV16(packet, 0x0017, ICQ_CLIENT_MAJOR);
	PackAddTLV16(packet, 0x0018, ICQ_CLIENT_MINOR);
	PackAddTLV16(packet, 0x0019, ICQ_CLIENT_BUILD1);
	PackAddTLV16(packet, 0x001a, ICQ_CLIENT_BUILD2);
	PackAddTLV32(packet, 0x0014, ICQ_CLIENT_BUILD3);
	
	/* Language and Country code */
	PackAddTLVStr(packet, 0x000f, "en");
	PackAddTLVStr(packet, 0x000e, "us");

	send_packet(0x01, packet);
}


extern void icq_connect(char *host, int port);

void parse_auth(char *data, int size) {
	
	short int type, length=0;
	char *uin, *host;
	int port;
	

	
	Packet *packet = PackNew();
	//memcpy(packet->data,data,size);
        packet->data=data;
	packet->size = size;
        
        char *tmp_msg=malloc(64);
        
        while (packet->offset<packet->size)
        {
          PackGetTLV(packet, &type, &length);
          switch (type)
          {
          case 0x01:
          uin = (char*)malloc(length + 1);
          PackGet(packet, (char*)uin, length);
          uin[length] = 0;
          break;
          case 0x04:
            //обработка ошибки
            
            
            Disconnect();
          return;
          case 0x05:
          /* Get new host */
            host = (char*)malloc(length + 1);
            PackGet(packet, (char*)host, length);
            host[length] = 0;
         
            int fl=0;
            int cs=0;
            char strport[5];
            for (int j=0;j<length;j++)
            {
              if (host[j]==':')
              {
                fl=1;
                cs=0;
              }
              else
              {
              if (fl)
                strport[cs++]=host[j]; 
              }
            }
            strport[cs]=0;
            
            port = str2int(strport);
            host=(char*)realloc(host, length-cs);
            host[length-cs-1]=0;
            _WriteLog(host);
          break;
          
          case 0x06:
          /* Get auth cookie */
          if (auth_cookie) mfree(auth_cookie);
          auth_cookie = (char*)malloc(length);
          auth_length=length;
          PackGet(packet, (char*)auth_cookie, length);
           
          _WriteLog("receiving auth cookie of length ");
           break;
           default:
             {
             char *tmp;
             tmp = malloc(length);
             PackGet(packet, (char*)tmp, length);
             mfree(tmp);
             }
          }
        }
        mfree(tmp_msg);
        
	Disconnect();
        
        if (auth_cookie)
        icq_connect(host, port);

        mfree(uin);
        mfree(host);
        
}

void send_cookie() 
{
   _WriteLog("send cookie");

   
        Packet *packet = PackNew();
	/* Raw start */
	PackAdd32(packet, 0x00000001);
	/* Auth cookie */
	PackAddTLVRAW(packet, 0x0006, auth_length, 
				      auth_cookie);
	send_packet( 0x01, packet);
}

void parse_snac(char *data, int size) {
	Packet *packet = PackNew();
	packet->data = data;
	packet->size = size;
	packet->offset = 0;
        
        SNAC_HEAD *snac = malloc(sizeof(SNAC_HEAD));
        memcpy(snac, data, sizeof(SNAC_HEAD));
   
        snac->service_id=htons(snac->service_id);
        snac->subtype_id=htons(snac->subtype_id);
        snac->flags=htons(snac->flags);
        snac->request_id=htonl(snac->request_id);
        
        packet->data = packet->data + 2+2+2+4;
        packet->size = size - sizeof(SNAC_HEAD);

	
          char *tmp_msg=malloc(64);
         sprintf(tmp_msg, "SNAC -> 0x%04x, 0x%04x", snac->service_id, snac->subtype_id);
         _WriteLog(tmp_msg);
         mfree(tmp_msg);

        switch (snac->service_id){
        case 0x0001:
          switch (snac->subtype_id)
            {
            case 0x0003:snac_server_ready(snac->flags, snac->request_id,packet);break;
//          case 0x0007:snac_rate_response(snac->flags, snac->request_id,packet);break;
            case 0x000f:snac_user_info(snac->flags, snac->request_id,packet);break;
            case 0x0013: snac_motd(snac->flags, snac->request_id,packet);break;
            case 0x0018: snac_host_versions(snac->flags, snac->request_id,packet);break;
//            case 0x001f: icq_snac_mem_request;break;
            }
          break;
        case 0x0002:
          switch (snac->subtype_id)
            {
            case 0x0003:snac_location_rights(snac->flags, snac->request_id,packet);break;
            }
          break;
        case 0x0003:
          switch (snac->subtype_id)
            {
            case 0x0003:snac_list_rights(snac->flags, snac->request_id,packet);break;
//		{ 0x0003, 0x000b, icq_snac_online_notify },
//		{ 0x0003, 0x000c, icq_snac_offline_notify },
            }
          break;  
        case 0x0004:
          switch (snac->subtype_id)
            {
            case 0x0005:snac_im_rights(snac->flags, snac->request_id,packet);break;
            case 0x0007:snac_incoming_msg(snac->flags, snac->request_id,packet);break;
            }
          break;  
        case 0x0009:
          switch (snac->subtype_id)
            {
            case 0x0003:snac_bos_rights(snac->flags, snac->request_id,packet);break;
            }
          break;  
        case 0x000b:
          switch (snac->subtype_id)
            {
//		{ 0x000b, 0x0002, icq_snac_report_interval },
            }
          break;  
        case 0x0013:
          switch (snac->subtype_id)
            {
	    case 0x0006:snac_contactlist(snac->flags, snac->request_id,packet);break;
            }
          break;    
        case 0x0015:
          switch (snac->subtype_id)
            {
	    case 0x0003:snac_server_message(snac->flags, snac->request_id,packet);break;
            }
          break;  

        }
	//n_error("Unhandled SNAC packet family 0x%04x, subtype 0x%04x:", family, subtype);
	//icq_packet_dump(&packet);
  
      
}

void Disconnect() 
{


}

void snac_new(Packet* packet,short int family, short int subtype, short int *flags, int reqid) 
{
	/* Add SNAC values before giving the packet away */
        //packet->size=0;  
        
          char *tmp_msg=malloc(64);
         sprintf(tmp_msg, "SNAC <- 0x%04x, 0x%04x", family, subtype);
         _WriteLog(tmp_msg);
         mfree(tmp_msg);
  
	PackAdd16(packet, family);
	PackAdd16(packet, subtype);
	
	if (flags)
		PackAdd(packet, (char *)flags, 2);
	else
		PackAdd16(packet, 0x0000);
	
	if (reqid)
          PackAdd32(packet, reqid);
//          *reqid = curr_reqid;
        else
	PackAdd32(packet, curr_reqid++);
	
	if (curr_reqid > 0xffffff00) curr_reqid = rand() % 47857;
	
}

void message_new(Packet* packet, short int type, short int *seqno) {
	PackAdd32LE(packet, UIN);
	PackAdd16(packet, type);
	/* ICQ Message Sequence number */
	PackAdd16LE(packet, icq_seqno);
	if (seqno) *seqno = icq_seqno;
	icq_seqno++;
}

void message_parse(Packet *packet) {  
	
  //ICQMessageHandler *handler = message_handlers;
	short int length, seqno, type, subtype = 0;
	int uin;
	
	PackGet16LE(packet, &length);
	PackGet32LE(packet, &uin);
	
	PackGet16(packet, &type);
	PackGet16LE(packet, &seqno);
	
	/* check to see if there is any single type handlers for this 
	 * packet */
/*
	for (; handler->type; handler++) {
		if (type == handler->type) {
			(*handler->func)(account, seqno, packet);
			return;
		}
	}
	
	/* If we don't have enough to check for a subtype then return */
/*	if (packet->size - packet->offset < sizeof(guint16)) {
		n_debug("Not enough remaining bytes to continue, unknown server message "
				"type (0x%04x), seqno (0x%04x):", type, seqno);
		icq_packet_dump(packet);
		return;
	}
	
	PackGet16(packet, subtype);
	
	/* Continue on again looking for both type and subtype */
/*	for (handler++; handler->type; handler++) {
		if (type == handler->type && subtype == handler->subtype) {
			(*handler->func)(account, seqno, packet);
			return;
		}
	}
	*/
//	n_debug("Unhandled packet type (0x%04x), subtype (0x%04x), seqno (0x%04x):",
//			type, subtype, seqno);
//	icq_packet_dump(packet);
}


void snac_server_ready(short int flags, int request_id, Packet *packet) 
{
	Packet *new_packet = PackNew();
        snac_new(new_packet,0x0001, 0x0017, NULL, NULL);
	
	_WriteLog("Server ready for session, sending versions");
	
	/* Send some versions packet */

	PackAdd32(new_packet, 0x00010004);
	PackAdd32(new_packet, 0x00130004);
	PackAdd32(new_packet, 0x00020001);
	PackAdd32(new_packet, 0x00030001);
	PackAdd32(new_packet, 0x00150001);
	PackAdd32(new_packet, 0x00040001);
	PackAdd32(new_packet, 0x00060001);
	PackAdd32(new_packet, 0x00090001);
	PackAdd32(new_packet, 0x000a0001);
	PackAdd32(new_packet, 0x000b0001);
       
       	
	send_packet(0x02, new_packet);
}


void snac_motd(short int flags, int request_id, Packet *packet) 
{
  Packet *new_packet = PackNew();
	snac_new(new_packet, 0x0013, 0x0005, NULL, NULL);
	PackAdd16(new_packet, 0x0000);
	PackAdd16(new_packet, 0x0000);
	PackAdd16(new_packet, 0x0000);
	send_packet(0x02, new_packet);
}


void snac_host_versions(short int flags, int request_id, Packet *packet) {
	Packet *new_packet = PackNew(); 
        
        snac_new(new_packet, 0x0001, 0x0006, NULL, NULL);
	
	_WriteLog("Server sent us the versions response");
	_WriteLog("Sending rate request");
	
	send_packet(0x02, new_packet);
        
}


void snac_rate_response(short int flags, int request_id, Packet *packet) {
	

   
  Packet *new_packet = PackNew();
  

	
	/* Acknowledge rate info */
	snac_new(new_packet, 0x0001, 0x0008, NULL, NULL);
	PackAdd16(new_packet, 0x0001);
	PackAdd16(new_packet, 0x0002);
	PackAdd16(new_packet, 0x0003);
	PackAdd16(new_packet, 0x0004);
	PackAdd16(new_packet, 0x0005);
	send_packet(0x02, new_packet);
	
	/* Request info about ourself */
	new_packet = PackNew();
        snac_new(new_packet, 0x0001, 0x000e, NULL, NULL);
	send_packet( 0x02, new_packet);
	
	/* Request location rights info */
	new_packet = PackNew();
        snac_new(new_packet, 0x0002, 0x0002, NULL, NULL);
	send_packet( 0x02, new_packet);
	
	/* Request list management rights info */
	new_packet = PackNew();
        snac_new(new_packet, 0x0003, 0x0002, NULL, NULL);
	send_packet( 0x02, new_packet);
	
	/* Request messaging rights info */
	new_packet = PackNew();
        snac_new(new_packet, 0x0004, 0x0004, NULL, NULL);
	send_packet( 0x02, new_packet);
	
	/* Request BOS rights info */
	new_packet = PackNew();
        snac_new(new_packet, 0x0009, 0x0002, NULL, NULL);
	send_packet( 0x02, new_packet);
        
        /*  
	new_packet = PackNew();
        snac_new(new_packet, 0x0013, 0x0002, NULL, NULL);
	send_packet( 0x02, new_packet);
        
         // запрос контакт листа
        new_packet = PackNew();
        snac_new(new_packet, 0x0013, 0x0004, NULL, NULL);
        send_packet( 0x02, new_packet);
        */
        
        snac_im_rights(0, 0,NULL);


        new_packet = PackNew();
        snac_new(new_packet, 0x0013, 0x0007, NULL, NULL);
        send_packet( 0x02, new_packet);
        
        snac_bos_rights(0, 0,NULL);
}


void snac_location_rights(short int flags, int request_id, Packet *packet) {
	_WriteLog("Recieved location rights");
}

void snac_list_rights(short int flags, int request_id, Packet *packet) {
	_WriteLog("Sending contact list");
	send_contact_list();
}

void snac_im_rights(short int flags, int request_id, Packet *packet) {


   
  Packet *new_packet = PackNew(); 
        snac_new(new_packet, 0x0004, 0x0002, NULL, 0x0002);
	
	/* Some Initial IM parameter */
	PackAdd16(new_packet, 0x0002);
	PackAdd32(new_packet, 0x00000003);
	PackAdd16(new_packet, 0x1f40);
	PackAdd16(new_packet, 0x03e7);
	PackAdd16(new_packet, 0x03e7);
	PackAdd32(new_packet, 0x00000000);
	
	_WriteLog("Sending initial IM stuff");
	
	send_packet(0x02, new_packet);
        
//        new_packet = PackNew(); 
//      snac_new(new_packet, 0x0013, 0x0005, NULL, NULL);
//        PackAdd32(new_packet,0);
//        PackAdd16(new_packet,0);
//	send_packet(0x02, new_packet);
  
}

void snac_bos_rights(short int flags, int request_id, Packet *packet) {

  
  SetStatus(STATUS_ONLINE);
  
    Packet *new_packet = PackNew();

    new_packet = PackNew();
        snac_new(new_packet, 0x0001, 0x0002, NULL, NULL);
	
	/* Send "Client Ready" */
	PackAdd32(new_packet, 0x00010003);
	PackAdd32(new_packet, 0x0110028A);
	PackAdd32(new_packet, 0x00020001);
	PackAdd32(new_packet, 0x0101028A);
	PackAdd32(new_packet, 0x00030001);
	PackAdd32(new_packet, 0x0110028A);
	PackAdd32(new_packet, 0x00150001);
	PackAdd32(new_packet, 0x0110028A);
	PackAdd32(new_packet, 0x00040001);
	PackAdd32(new_packet, 0x0110028A);
	PackAdd32(new_packet, 0x00060001);
	PackAdd32(new_packet, 0x0110028A);
	PackAdd32(new_packet, 0x00090001);
	PackAdd32(new_packet, 0x0110028A);
	PackAdd32(new_packet, 0x000a0003);
	PackAdd32(new_packet, 0x0110028A);
	
	_WriteLog("Sending Client Ready");
	
	send_packet( 0x02, new_packet);
        
       
        
         _WriteLog("STATUS_ONLINE");

        
	
        // запрос контакт листа
      //  new_packet = PackNew();
      //  snac_new(new_packet, 0x0013, 0x0004, NULL, NULL);
      //  send_packet( 0x02, new_packet);
        
        // запрос оффлайн сообщений
        new_packet = PackNew();
        message_new(new_packet, 0x3c00, NULL);
	send_message(new_packet);
        
        
	//send_key_data("<key>DataFilesIP</key>");
	//send_key_data("<key>BannersIP</key>");
	//send_key_data("<key>ChannelsIP</key>");
        
       // process_active_timer();
       // process_recv_timer();
}

void send_key_data(char *data) {
	Packet *packet = PackNew();
        message_new(packet, 0xd007, NULL);
	int size = strlen(data) + 1;
	
	/* Subtype */
	PackAdd16(packet, 0x9808);
	/* Key size */
	PackAdd16LE(packet, size);
	/* actual data */
	PackAdd(packet, data, size);
	
	send_message(packet);
}

void snac_server_message(short int flags, int request_id, Packet *packet) {
	short int tlv_type, tlv_length;
	
	PackGetTLV(packet, &tlv_type, &tlv_length);
	
	if (tlv_type != 0x0001) return;
	
        message_parse(packet);
}

void send_contact_list() {
	Packet *packet = PackNew();
	
//	if (!ACCOUNT(account)->contacts) return;
	
	snac_new(packet, 0x0003, 0x0004, NULL, NULL);
	
	/* Build a packet with all contacts in it */
//	g_list_foreach(ACCOUNT(account)->contacts, (GFunc)icq_send_contact_cb, packet);
        /*
        for (int i=0;i<ContactList::Active->list->GetCount();i++)
        {
          CLIST* item=ContactList::Active->list->GetItem(i);
          if (!item->isgroup)
          {
            char *str=(char*)malloc(64);
            sprintf(str,percent_d,item->uin);
            PackAdd8(packet,(char)strlen(str));
            PackAddStr(packet,str);
          }
        }
	*/
	send_packet( 0x02, packet);
}

void snac_user_info(short int flags, int request_id, Packet *packet) {
	short int warn_level, tlv_count;
	char user_len;
	char *user;
	
        
	/* username length */
	PackGet8(packet, &user_len);
	/* username */
        user = (char*)malloc((int)user_len + 1);
        PackGet(packet, (char*)user, (int)user_len);
        user[(int)user_len] = 0;
        
	//PackGetStr(packet, user, (int)user_len);
	
	/* Warning level */
	PackGet16(packet, &warn_level);
	PackGet16(packet, &tlv_count);
	
	//n_debug("User info for %s: Warning level = %lu (%lu remaining TLVs)",
	//		user, warn_level, tlv_count);
	
	/* Only set our info if it's the first time we receive this packet */
//	if (Status == CONNECTING) 
        send_user_info();
}

void send_user_info() {
  
         // Set User Info (capability)
  
	Packet *packet = PackNew();
        snac_new(packet, 0x0002, 0x0004, NULL, NULL);
	
	PackAddTLV(packet, 0x0005, 0x0010);
	
	/* This is the capability block for the icq client */
	PackAdd32(packet, 0x09461349);
	PackAdd32(packet, 0x4c7f11d1);
	PackAdd32(packet, 0x82224445);
	PackAdd32(packet, 0x53540000);
	
	send_packet( 0x02, packet);
}

void snac_incoming_msg(short int flags, int request_id, Packet *packet) {
	//guint8 cookie[8], source_len;
	//guint16 channel, warning_level, num_tlvs;
	char *source;
	
	/* IM Cookie */
	//icq_packet_get(packet, cookie, 8);
	
	/* The IM channel that this is coming in on */
	/*
        PACKET_GET_16(packet, channel);
	
	PACKET_GET_8(packet, source_len);
	PACKET_GET_STR(packet, source, source_len);
	
	PACKET_GET_16(packet, warning_level);
	PACKET_GET_16(packet, num_tlvs);
	
	n_debug("Incoming Message:");
	icq_packet_dump(packet);
	
	switch (channel) {
	  case 0x0001:
		handle_simple_message(account, source, num_tlvs, packet);
		break;
		
	  case 0x0002:
		handle_advanced_message(account, source, num_tlvs, packet);
		break;
		
	  case 0x0004:
		handle_special_message(account, source, num_tlvs, packet);
		break;
		
	  default:
		n_error("Unknown IM channel 0x%04x", channel);
		icq_packet_dump(packet);
		break;
	}
	
	d_free(source);
        */
}

void snac_contactlist(short int flags, int request_id, Packet *packet) {

  /*
        //запрос оффлайн сообщений
        Packet *new_packet = PackNew();
        message_new(new_packet, 0x3c00, NULL);
	send_message(new_packet);
        

       
        // устанавливаем статус
        new_packet = PackNew();
        snac_new(new_packet, 0x0001, 0x001e, NULL, NULL);
        PackAddTLV32(new_packet, 0x0006, 0x00000001);
	send_packet(0x02, new_packet);
   */   
  
snac_im_rights(0, 0,NULL);


//        Packet *new_packet = PackNew();
//        snac_new(new_packet, 0x0013, 0x0007, NULL, 0x0007);
//        send_packet( 0x02, new_packet);
        
snac_bos_rights(0, 0,NULL);
        
  
        return;
//Version number of SSI protocol (currently 0x00)
	char vnum;
	PackGet8(packet, &vnum);
        _WriteLog("parse CL");
        short int Count=0;
        PackGet16(packet,&Count);
       // if (Count<=0) return;
        for (int i=0;i<Count;i++)
        {
          short int len=0;
          PackGet16(packet,&len);
          char *name;
          char *uin_name=(char*)malloc(64);

          name = (char*)malloc((int)len + 1);
          PackGet(packet, (char*)name, (int)len);
          name[(int)len] = 0;
          short int GroupID=0;
          PackGet16(packet,&GroupID);
          short int ItemID=0;
          PackGet16(packet,&ItemID);
          short int Type=0;
          PackGet16(packet,&Type);
          PackGet16(packet,&len);
          
          short int type, length=0;
          int end_info=packet->offset+len;
          
          while (packet->offset<end_info)
            {
              PackGetTLV(packet, &type, &length);
              switch (type)
              {
              case 0x0131:
                PackGet(packet, (char*)uin_name, length);
                uin_name[(int)length]=0;
                break;
              default:
                { 
                char *tmp = malloc(length);
                 PackGet(packet, (char*)tmp, length);
                 mfree(tmp);
                }
              }
            }
          
          switch (Type)
          {
          case 0:// buddi
//            int uin = str2int(name);
//            ContactList::Active->list->AddUser((char*)uin_name,0,uin,GroupID);
            break;
          case 1:// group
//            ContactList::Active->list->AddUser(name,1,0,GroupID);
            break;
          default:
            break;
          }
          
          mfree(uin_name);
          
        }
        
        
}



void Keep_alive()
{
  Packet* pack=PackNew();
  send_packet( 0x05, pack);
//  LogWidget::Active->Redraw();
}


void SetStatus(int Status)
{
       ICQStatus = Status;
     
       /*
       // Set Status Code
       tmp := CreatePacket(2,SEQ);
       SNACAppend(tmp,$1,$1E);
       TLVAppendDWord(tmp,6,ICQStatus);
       TLVAppendWord(tmp,8,$0000);
       // imitation TLV(C)
       PacketAppend32(tmp,dswap($000C0025)); // TLV(C)
       StrToIP(Get_my_IP,DIM_IP);
       PacketAppend(tmp,@DIM_IP,4);                  // IP address
       PacketAppend32(tmp,dswap(28000+random(1000)));// Port
       PacketAppend8(tmp,$04);
       PacketAppend16(tmp,swap($0007));
       PacketAppend16(tmp,swap($466B));
       PacketAppend16(tmp,swap($AE68));
       PacketAppend32(tmp,dswap($00000050));
       PacketAppend32(tmp,dswap($00000003));
       PacketAppend32(tmp,dswap(SecsSince1970));
       PacketAppend32(tmp,dswap(SecsSince1970));
       PacketAppend32(tmp,dswap(SecsSince1970));
       PacketAppend16(tmp,swap($0000));
       PacketSend(tmp); 
       */
       int long_status=0;
       
       long_status=flags_status*0x10000+Status;
       
       
       Packet *new_packet = PackNew();
       snac_new(new_packet, 0x0001, 0x001e, NULL, NULL);
       PackAddTLV32(new_packet, 0x0006, long_status);
       //PackAddTLV16(new_packet, 0x0008, 0x0000);
      
       
       send_packet(0x02, new_packet);
}
