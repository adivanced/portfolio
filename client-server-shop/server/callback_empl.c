#include <unistd.h>
#include <stdlib.h>
#include <string.h>

extern void* windows_empl[20];


struct button_empl{
	float r;
	float g;
	float b;
	float x;
	float y;
	float tr;
	float tg;
	float tb;
	float length;
	float heigth;
	float sr;
	float sg;
	float sb;
	uint8_t font_ptr;
	uint8_t button_r_type;
}__attribute__((packed));

struct text_empl{
	float r;
	float g;
	float b;
	float x;
	float y;
	uint8_t font_ptr;
}__attribute__((packed));

struct box_empl{
	float r;
	float g;
	float b;
	float x;
	float y;
	float length;
	float heigth;
}__attribute__((packed));




uint64_t client_id_empl = 0xFFFFFFFFFFFFFFFF;
uint64_t curpage_cart_empl = 0;
uint64_t curpage_chat_empl = 0;
uint64_t curpage_clnt_empl = 0;
uint64_t sel_chat = 0;

uint64_t calb_6_ghost(void* buf, uint64_t n, PGconn* db_conn, int sockfd){
	char cid[20];
	sprintf(cid, "%ld", sel_chat);

	printf("USER ID: %s\n", cid);

	sel_chat = atoll(cid);


	memcpy(buf, windows_empl[4]+8, *((uint64_t*)windows_empl[4])-8-15);

	PGresult* res_chid = PQexecParams(db_conn, "SELECT \"Chat\".\"Chat_Id\" FROM \"Chat\" WHERE \"Chat\".\"Client_Id\" = $1;", 1, NULL, (const char*[]){cid}, NULL, NULL, 0);

	char prelude_entrys[20];
	sprintf(prelude_entrys, "%ld", curpage_chat_empl*9);

	PGresult* res_mesg = PQexecParams(db_conn, "SELECT \"Text\", \"Employee_id\" FROM \"Message\" WHERE \"Message\".\"Message_Id\" NOT IN (SELECT \"Message\".\"Message_Id\" FROM \"Message\" WHERE \"Chat_Id\" = $1 ORDER BY \"Message\".\"Message_Id\" DESC limit $2) AND \"Chat_Id\" = $1 ORDER BY \"Message\".\"Message_Id\" DESC limit 9;", 2, NULL, (const char*[]){PQgetvalue(res_chid, 0, 0), prelude_entrys}, NULL, NULL, 0);
	uint64_t n_mesg = PQntuples(res_mesg);

	PGresult* res_totl = PQexecParams(db_conn, "SELECT COUNT(*) FROM \"Message\" WHERE \"Message\".\"Chat_Id\" = $1;", 1, NULL, (const char*[]){PQgetvalue(res_chid, 0, 0)}, NULL, NULL, 0);
	uint64_t total_n = atoll(PQgetvalue(res_totl, 0, 0));

	void* ptr = buf+*((uint64_t*)windows_empl[4])-8-15;

	if(curpage_chat_empl){
		*((struct button*)ptr) = (struct button){r:0.0f, g:0.0f, b:1.0f, x:0.9f, y:0.0f, tr:0.0f, tg:0.0f, tb:0.0f, length:0.1f, heigth:0.1f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:6, button_r_type:14};  // prev button
		ptr += sizeof(struct button);
		(*((uint8_t*)buf+1))++;	
	}

	if((curpage_chat_empl+1 != total_n/9 + !(total_n%9 == 0)) && total_n != 0) {
		*((struct button*)ptr) = (struct button){r:0.0f, g:0.0f, b:1.0f, x:0.9f, y:0.2f, tr:0.0f, tg:0.0f, tb:0.0f, length:0.1f, heigth:0.1f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:6, button_r_type:15};  // prev button
		ptr += sizeof(struct button);
		(*((uint8_t*)buf+1))++;			
	}

	memcpy(ptr, windows_empl[4]+*((uint64_t*)windows_empl[4])-15, 15);
	ptr += 15;


	if(curpage_chat_empl){
		memcpy(ptr, "\\/", 3); 
		ptr += 3;
	}

	if(curpage_chat_empl+1 != total_n/9 + !(total_n%9 == 0) && total_n != 0){
		memcpy(ptr, "/\\", 3);
		ptr += 3;
	}

	float yi = -0.7f;
	for(uint64_t i = 0; i < n_mesg; i++){
		if(!atoll(PQgetvalue(res_mesg, i, 1))){ // sent by employee
			*((struct box*)ptr) = (struct box){r:1.0f, g:1.0f, b:1.0f, x:-0.9f, y:yi, length:1.8f, heigth:0.1f};
			ptr += sizeof(struct box);
			*((struct box*)ptr) = (struct box){r:1.0f, g:1.0f, b:1.0f, x:-1.0f, y:yi-0.1f, length:0.1f, heigth:0.025f};
			(*((uint8_t*)buf+3))+=2;
			ptr += sizeof(struct box);
		}else{
			*((struct box*)ptr) = (struct box){r:0.196f, g:0.803f, b:0.196f, x:-0.9f, y:yi, length:1.8f, heigth:0.1f};
			ptr += sizeof(struct box);
			*((struct box*)ptr) = (struct box){r:0.196f, g:0.803f, b:0.196f, x:0.9f, y:yi-0.1f, length:0.1f, heigth:0.025f};
			(*((uint8_t*)buf+3))+=2;			
			ptr += sizeof(struct box);
		}
		yi += 0.2;
	}

	yi = -0.7;
	for(uint64_t i = 0; i < n_mesg; i++){
		*((struct text*)ptr) = (struct text){r:0.0f, g:0.0f, b:0.0f, x:-0.9f, y:yi-0.05f, font_ptr:6};
		(*((uint8_t*)buf+2))+=1;
		ptr += sizeof(struct text);
		yi += 0.2;
	}

	for(uint64_t i = 0; i < n_mesg; i++){
		char* txt = PQgetvalue(res_mesg, i, 0);
		//printf("MSG: %s\n", txt);
		uint64_t len = strlen(txt);
		memcpy(ptr, txt, len+1);
		ptr += len+1;
	}


	send(sockfd, buf, ptr-buf, 0);
	return -1;	
}



uint64_t callback_0_empl(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // exit callback
	printf("Disconnected!\n");
	_exit(0);
}

uint64_t callback_1_empl(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // login employee callback
	char* emplname = buf;
	char* emplpass = buf + strlen(buf)+1;
	printf("EMPLNAME: %s EMPPASS %s\n", emplname, emplpass);

	PGresult* res_empl = PQexecParams(db_conn, "SELECT \"Employee_id\" FROM \"Employee\" WHERE \"Login\" = $1 AND \"Password\" = $2;", 2, NULL, (const char*[]){emplname, emplpass}, NULL, NULL, 0);
	if(PQresultStatus(res_empl) == PGRES_FATAL_ERROR){return 1;}
	if(!PQntuples(res_empl)){return 1;}


	client_id_empl = atoll(PQgetvalue(res_empl, 0, 0));

	return 2;
}

uint64_t callback_2_empl(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // client list
	//printf("ENTER CALB 2 %ld\n", *((uint64_t*)windows_empl[3]));
	memcpy(buf, windows_empl[3]+8, *((uint64_t*)windows_empl[3])-10-8);

	void* ptr = buf + *((uint64_t*)windows_empl[3])-10-8;

	char prelude_entrys[20];
	sprintf(prelude_entrys, "%ld", curpage_clnt_empl*5);

	PGresult* res_clients = PQexecParams(db_conn, "SELECT \"Client\".\"Name\", \"Client\".\"Date_Of_Birth\", \"Client\".\"Phone_Number\", \"Login\".\"Login\", \"Login\".\"Password\" FROM \"Client\" JOIN \"Login\" ON \"Client\".\"Login_Data_Id\" = \"Login\".\"Pass_Id\" WHERE \"Client\".\"Client_Id\" NOT IN (SELECT \"Client\".\"Client_Id\" FROM \"Client\" ORDER BY \"Client\".\"Client_Id\" limit $1) ORDER BY \"Client\".\"Client_Id\" LIMIT 5;", 1, NULL, (const char*[]){prelude_entrys}, NULL, NULL, 0);

	uint64_t n_clients = PQntuples(res_clients);

	float yi = 0.9f;
	for(uint64_t i = 0; i < n_clients; i++){
		*((struct button*)ptr) = (struct button){r:1.0f, g:0.0f, b:0.0f, x:0.85f, y:yi, tr:0.0f, tg:0.0f, tb:0.0f, length:0.15f, heigth:0.2f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:6, button_r_type:5}; // delete client button
		ptr += sizeof(struct button);
		(*((uint8_t*)buf+1))++;

		*((struct button*)ptr) = (struct button){r:0.6f, g:0.8f, b:0.9f, x:0.75f, y:yi, tr:0.0f, tg:0.0f, tb:0.0f, length:0.1f, heigth:0.2f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:6, button_r_type:6}; // chat client button
		ptr += sizeof(struct button);
		(*((uint8_t*)buf+1))++;

		yi -= 0.3f;
	}

	memcpy(ptr, "Exit\0Back", 10);
	ptr += 10;

	for(uint64_t i = 0; i < n_clients; i++){
		memcpy(ptr, "Delete\0Chat", 12);
		ptr += 12;
	}

	yi = 0.9f;
	for(uint64_t i = 0; i < n_clients; i++){
		*((struct box*)ptr) = (struct box){r:1.0f, g:1.0f, b:0.0f, x:-1.0f, y:yi, length:1.75f, heigth:0.2f};
		ptr += sizeof(struct box);
		(*((uint8_t*)(buf+3)))++;	
		yi -= 0.3f;
	}


	yi = 0.9f;
	for(uint64_t i = 0; i < n_clients; i++){
		*((struct text*)ptr) = (struct text){r:0.0, g:0.0, b:0.0, x:-0.9, y:yi-0.05, font_ptr:6};
		(*((uint8_t*)(buf+2)))++;
		ptr += sizeof(struct text);		

		*((struct text*)ptr) = (struct text){r:0.0, g:0.0, b:0.0, x:-0.9, y:yi-0.15f, font_ptr:6};
		(*((uint8_t*)(buf+2)))++;
		ptr += sizeof(struct text);		

		yi -= 0.3f;
	}

	for(uint64_t i = 0; i < n_clients; i++){
		char* name = PQgetvalue(res_clients, i, 0);
		char* date = PQgetvalue(res_clients, i, 1);
		char* phon = PQgetvalue(res_clients, i, 2);
		char* logn = PQgetvalue(res_clients, i, 3);
		char* pasw = PQgetvalue(res_clients, i, 4);

		memcpy(ptr, "Login: ", 7);
		ptr += 7;
		memcpy(ptr, logn, strlen(logn));
		ptr += strlen(logn);
		memcpy(ptr, " | Passw: ", 10);
		ptr += 10;
		memcpy(ptr, pasw, strlen(pasw)+1);
		ptr += strlen(pasw)+1;

		memcpy(ptr, "Name: ", 6);
		ptr += 6;
		memcpy(ptr, name, strlen(name));
		ptr += strlen(name);
		memcpy(ptr, " | Phone: ", 10);
		ptr += 10;
		memcpy(ptr, phon, strlen(phon));
		ptr += strlen(phon);
		memcpy(ptr, " | Birth Date: ", 15);
		ptr += 15;
		memcpy(ptr, date, strlen(date)+1);
		ptr += strlen(date)+1;
	}

	send(sockfd, buf, ptr-buf, 0);
	return -1;

}

uint64_t callback_3_empl(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // order list
	memcpy(buf, windows_empl[3]+8, *((uint64_t*)windows_empl[3])-10-8);
	void* ptr = buf + *((uint64_t*)windows_empl[3])-10-8;

	char prelude_entrys[20];
	sprintf(prelude_entrys, "%ld", curpage_cart_empl*9);

	PGresult* res_totaln = PQexec(db_conn, "SELECT COUNT(*) FROM \"Order\";");
	uint64_t total_n = atoll(PQgetvalue(res_totaln, 0, 0));

	PGresult* res_orders = PQexecParams(db_conn, "SELECT \"Order\".\"Order_Id\" FROM \"Order\" WHERE \"Order_Id\" NOT IN (SELECT \"Order\".\"Order_Id\" FROM \"Order\" ORDER BY \"Order\".\"Order_Id\" limit $1) ORDER BY \"Order\".\"Order_Id\" LIMIT 9;", 1, NULL, (const char*[]){prelude_entrys}, NULL, NULL, 0);

	uint64_t n_orders = PQntuples(res_orders);

	float yi = 0.9f;
	for(uint64_t i = 0; i < n_orders; i++){
		*((struct button*)ptr) = (struct button){r:1.0f, g:0.0f, b:0.0f, x:0.85f, y:yi, tr:0.0f, tg:0.0f, tb:0.0f, length:0.15f, heigth:0.1f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:6, button_r_type:10}; // delete order button
		ptr += sizeof(struct button);
		(*((uint8_t*)buf+1))++;

		PGresult* res_status = PQexecParams(db_conn, "SELECT \"Order\".\"Status_Id\" FROM \"Order\" WHERE \"Order_Id\" = $1;", 1, NULL, (const char*[]){PQgetvalue(res_orders, i, 0)}, NULL, NULL, 0);
		if(atoll(PQgetvalue(res_status, 0, 0)) != 1 && atoll(PQgetvalue(res_status, 0, 0)) != 6){
			*((struct button*)ptr) = (struct button){r:0.0f, g:1.0f, b:0.0f, x:0.75f, y:yi, tr:0.0f, tg:0.0f, tb:0.0f, length:0.1f, heigth:0.1f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:6, button_r_type:9}; // approve order button
			ptr += sizeof(struct button);
			(*((uint8_t*)buf+1))++;
		}

		yi -= 0.2f;
	}

	if(curpage_cart_empl){
		*((struct button*)ptr) = (struct button){r:0.0f, g:0.0f, b:1.0f, x:-0.1f, y:-0.9f, tr:0.0f, tg:0.0f, tb:0.0f, length:0.1f, heigth:0.1f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:6, button_r_type:12};  // prev button
		ptr += sizeof(struct button);
		(*((uint8_t*)buf+1))++;	
	}

	if((curpage_cart_empl+1 != total_n/9 + !(total_n%9 == 0)) && total_n != 0) {
		*((struct button*)ptr) = (struct button){r:0.0f, g:0.0f, b:1.0f, x:0.1f, y:-0.9f, tr:0.0f, tg:0.0f, tb:0.0f, length:0.1f, heigth:0.1f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:6, button_r_type:11};  // next button
		ptr += sizeof(struct button);
		(*((uint8_t*)buf+1))++;			
	}


	memcpy(ptr, "Exit\0Back", 10);
	ptr += 10;

	for(uint64_t i = 0; i < n_orders; i++){
		memcpy(ptr, "Delete", 7);
		ptr += 7;
		PGresult* res_status = PQexecParams(db_conn, "SELECT \"Order\".\"Status_Id\" FROM \"Order\" WHERE \"Order_Id\" = $1;", 1, NULL, (const char*[]){PQgetvalue(res_orders, i, 0)}, NULL, NULL, 0);
		if(atoll(PQgetvalue(res_status, 0, 0)) != 1 && atoll(PQgetvalue(res_status, 0, 0)) != 6){
			memcpy(ptr, "/\\", 3);
			ptr += 3;
		}
	}

	if(curpage_cart_empl){
		memcpy(ptr, "<-", 3);
		ptr += 3;
	}

	if((curpage_cart_empl+1 != total_n/9 + !(total_n%9 == 0)) && total_n != 0) {
		memcpy(ptr, "->", 3);
		ptr += 3;
	}


	yi = 0.9f;
	for(uint64_t i = 0; i < n_orders; i++){
		*((struct box*)ptr) = (struct box){r:1.0f, g:1.0f, b:0.0f, x:-1.0f, y:yi, length:1.75f, heigth:0.1f};
		ptr += sizeof(struct box);
		(*((uint8_t*)(buf+3)))++;	
		yi -= 0.2f;		
	}

	yi = 0.9f;
	for(uint64_t i = 0; i < n_orders; i++){
		*((struct text*)ptr) = (struct text){r:0.0, g:0.0, b:0.0, x:-0.9, y:yi-0.05, font_ptr:6};
		(*((uint8_t*)(buf+2)))++;
		ptr += sizeof(struct text);				
		yi -= 0.2;
	}

	for(uint64_t i = 0; i < n_orders; i++){
		PGresult* res_am = PQexecParams(db_conn, "SELECT \"Amount_Of_Beer\".\"Beer_Id\", \"Amount_Of_Beer\".\"Amount_Of_Beer\" FROM \"Amount_Of_Beer\" WHERE \"Order_Id\" = $1;", 1, NULL, (const char*[]){PQgetvalue(res_orders, i, 0)}, NULL, NULL, 0);
		char* amount = PQgetvalue(res_am, 0, 1);
		PGresult* res_beer = PQexecParams(db_conn, "SELECT \"Beer\".\"Beer_Name\" FROM \"Beer\" WHERE \"Beer\".\"Beer_Id\" = $1", 1, NULL, (const char*[]){PQgetvalue(res_am, 0, 0)}, NULL, NULL, 0);
		char* beer = PQgetvalue(res_beer, 0, 0);
		PGresult* res_status = PQexecParams(db_conn, "SELECT \"Status\".\"Current_Status\" FROM \"Status\" WHERE \"Status_Id\" = (SELECT \"Order\".\"Status_Id\" FROM \"Order\" WHERE \"Order_Id\" = $1);", 1, NULL, (const char*[]){PQgetvalue(res_orders, i, 0)}, NULL, NULL, 0);
		char* status = PQgetvalue(res_status, 0, 0);

		memcpy(ptr, "Item: ", 6);
		ptr += 6; 
		memcpy(ptr, beer, strlen(beer));
		ptr += strlen(beer);
		memcpy(ptr, " Amount: ", 9);
		ptr += 9;
		memcpy(ptr, amount, strlen(amount));
		ptr += strlen(amount);
		memcpy(ptr, " Status: ", 9);
		ptr += 9;
		memcpy(ptr, status, strlen(status)+1);
		ptr += strlen(status)+1;
	}

	send(sockfd, buf, ptr-buf, 0);
	return -1;

}

uint64_t callback_4_empl(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // back button
	if(client_id_empl != 0xFFFFFFFFFFFFFFFF){
		curpage_cart_empl = 0;
		curpage_chat_empl = 0;
 		curpage_clnt_empl = 0;

 		return 2;
 	}
}

uint64_t callback_5_empl(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // delete client
	uint64_t f = atoll(buf);

	char prelude_entrys[20];
	sprintf(prelude_entrys, "%ld", curpage_clnt_empl*5);

	PGresult* res_clients = PQexecParams(db_conn, "SELECT \"Client\".\"Client_Id\" FROM \"Client\" WHERE \"Client\".\"Client_Id\" NOT IN (SELECT \"Client\".\"Client_Id\" FROM \"Client\" ORDER BY \"Client\".\"Client_Id\" limit $1) ORDER BY \"Client\".\"Client_Id\" LIMIT 5;", 1, NULL, (const char*[]){prelude_entrys}, NULL, NULL, 0);

	PGresult* fin_res = PQexecParams(db_conn, "CALL delete_client($1);", 1, NULL, (const char*[]){PQgetvalue(res_clients, f, 0)}, NULL, NULL, 0);

	if(PQresultStatus(fin_res) == PGRES_FATAL_ERROR){printf("FAILED TO DELETE CLIENT %s\n", PQgetvalue(res_clients, f, 0));}

	return callback_2_empl(buf, n, db_conn, sockfd);
}


uint64_t callback_6_empl(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // chat w client
	char* unm = buf;
	char* pwd = unm+strlen(unm)+1;

	printf("unm:%s pwd:%s\n", unm, pwd);

	PGresult* res_cid = PQexecParams(db_conn, "SELECT \"Client\".\"Client_Id\" FROM \"Client\" JOIN \"Login\" ON \"Client\".\"Login_Data_Id\" = \"Login\".\"Pass_Id\" WHERE \"Login\".\"Login\" = $1 AND \"Login\".\"Password\" = $2;", 2, NULL, (const char*[]){unm, pwd}, NULL, NULL, 0);
	char* cid = PQgetvalue(res_cid, 0, 0);

	printf("USER ID: %s\n", cid);

	sel_chat = atoll(cid);


	memcpy(buf, windows_empl[4]+8, *((uint64_t*)windows_empl[4])-8-15);

	PGresult* res_chid = PQexecParams(db_conn, "SELECT \"Chat\".\"Chat_Id\" FROM \"Chat\" WHERE \"Chat\".\"Client_Id\" = $1;", 1, NULL, (const char*[]){cid}, NULL, NULL, 0);

	char prelude_entrys[20];
	sprintf(prelude_entrys, "%ld", curpage_chat_empl*9);

	PGresult* res_mesg = PQexecParams(db_conn, "SELECT \"Text\", \"Employee_id\" FROM \"Message\" WHERE \"Message\".\"Message_Id\" NOT IN (SELECT \"Message\".\"Message_Id\" FROM \"Message\" WHERE \"Chat_Id\" = $1 ORDER BY \"Message\".\"Message_Id\" DESC limit $2) AND \"Chat_Id\" = $1 ORDER BY \"Message\".\"Message_Id\" DESC limit 9;", 2, NULL, (const char*[]){PQgetvalue(res_chid, 0, 0), prelude_entrys}, NULL, NULL, 0);
	uint64_t n_mesg = PQntuples(res_mesg);

	PGresult* res_totl = PQexecParams(db_conn, "SELECT COUNT(*) FROM \"Message\" WHERE \"Message\".\"Chat_Id\" = $1;", 1, NULL, (const char*[]){PQgetvalue(res_chid, 0, 0)}, NULL, NULL, 0);
	uint64_t total_n = atoll(PQgetvalue(res_totl, 0, 0));

	void* ptr = buf+*((uint64_t*)windows_empl[4])-8-15;

	if(curpage_chat_empl){
		*((struct button*)ptr) = (struct button){r:0.0f, g:0.0f, b:1.0f, x:0.9f, y:0.0f, tr:0.0f, tg:0.0f, tb:0.0f, length:0.1f, heigth:0.1f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:6, button_r_type:14};  // prev button
		ptr += sizeof(struct button);
		(*((uint8_t*)buf+1))++;	
	}

	if((curpage_chat_empl+1 != total_n/9 + !(total_n%9 == 0)) && total_n != 0) {
		*((struct button*)ptr) = (struct button){r:0.0f, g:0.0f, b:1.0f, x:0.9f, y:0.2f, tr:0.0f, tg:0.0f, tb:0.0f, length:0.1f, heigth:0.1f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:6, button_r_type:15};  // prev button
		ptr += sizeof(struct button);
		(*((uint8_t*)buf+1))++;			
	}

	memcpy(ptr, windows_empl[4]+*((uint64_t*)windows_empl[4])-15, 15);
	ptr += 15;


	if(curpage_chat_empl){
		memcpy(ptr, "\\/", 3); 
		ptr += 3;
	}

	if(curpage_chat_empl+1 != total_n/9 + !(total_n%9 == 0) && total_n != 0){
		memcpy(ptr, "/\\", 3);
		ptr += 3;
	}

	float yi = -0.7f;
	for(uint64_t i = 0; i < n_mesg; i++){
		if(!atoll(PQgetvalue(res_mesg, i, 1))){ // sent by employee
			*((struct box*)ptr) = (struct box){r:1.0f, g:1.0f, b:1.0f, x:-0.9f, y:yi, length:1.8f, heigth:0.1f};
			ptr += sizeof(struct box);
			*((struct box*)ptr) = (struct box){r:1.0f, g:1.0f, b:1.0f, x:-1.0f, y:yi-0.1f, length:0.1f, heigth:0.025f};
			(*((uint8_t*)buf+3))+=2;
			ptr += sizeof(struct box);
		}else{
			*((struct box*)ptr) = (struct box){r:0.196f, g:0.803f, b:0.196f, x:-0.9f, y:yi, length:1.8f, heigth:0.1f};
			ptr += sizeof(struct box);
			*((struct box*)ptr) = (struct box){r:0.196f, g:0.803f, b:0.196f, x:0.9f, y:yi-0.1f, length:0.1f, heigth:0.025f};
			(*((uint8_t*)buf+3))+=2;			
			ptr += sizeof(struct box);
		}
		yi += 0.2;
	}

	yi = -0.7;
	for(uint64_t i = 0; i < n_mesg; i++){
		*((struct text*)ptr) = (struct text){r:0.0f, g:0.0f, b:0.0f, x:-0.9f, y:yi-0.05f, font_ptr:6};
		(*((uint8_t*)buf+2))+=1;
		ptr += sizeof(struct text);
		yi += 0.2;
	}

	for(uint64_t i = 0; i < n_mesg; i++){
		char* txt = PQgetvalue(res_mesg, i, 0);
		//printf("MSG: %s\n", txt);
		uint64_t len = strlen(txt);
		memcpy(ptr, txt, len+1);
		ptr += len+1;
	}


	send(sockfd, buf, ptr-buf, 0);
	return -1;
}

uint64_t callback_7_empl(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // prev page client
	curpage_clnt_empl--;
	return callback_2_empl(buf, n, db_conn, sockfd);
}

uint64_t callback_8_empl(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // next page client
	curpage_clnt_empl++;
	return callback_2_empl(buf, n, db_conn, sockfd);
}

uint64_t callback_9_empl(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // approve order
	uint64_t f = atoll(buf);
	
	printf("NORDER: %ld\n", f);

	char prelude_entrys[20];
	sprintf(prelude_entrys, "%ld", curpage_cart_empl*9);

	PGresult* res_orders = PQexecParams(db_conn, "SELECT \"Order\".\"Order_Id\" FROM \"Order\" WHERE \"Order_Id\" NOT IN (SELECT \"Order\".\"Order_Id\" FROM \"Order\" ORDER BY \"Order\".\"Order_Id\" limit $1) ORDER BY \"Order\".\"Order_Id\" LIMIT 9;", 1, NULL, (const char*[]){prelude_entrys}, NULL, NULL, 0);

	char* res_id = PQgetvalue(res_orders, f, 0);

	printf("RESID %s\n", res_id);

	PGresult* res_status = PQexecParams(db_conn, "SELECT \"Order\".\"Status_Id\" FROM \"Order\" WHERE \"Order\".\"Order_Id\" = $1;", 1, NULL, (const char*[]){res_id}, NULL, NULL, 0);

	uint64_t stid = atoll(PQgetvalue(res_status, 0, 0))-1;

	char stids[20];
	sprintf(stids, "%ld", stid);

	PGresult* res_fin = PQexecParams(db_conn, "UPDATE \"Order\" SET \"Status_Id\" = $1 WHERE \"Order\".\"Order_Id\" = $2;", 2, NULL, (const char*[]){stids, res_id}, NULL, NULL, 0);

	return callback_3_empl(buf, n, db_conn, sockfd);

}

uint64_t callback_10_empl(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // delete order
	uint64_t f = atoll(buf);

	char prelude_entrys[20];
	sprintf(prelude_entrys, "%ld", curpage_cart_empl*9);

	PGresult* res_orders = PQexecParams(db_conn, "SELECT \"Order\".\"Order_Id\" FROM \"Order\" WHERE \"Order_Id\" NOT IN (SELECT \"Order\".\"Order_Id\" FROM \"Order\" ORDER BY \"Order\".\"Order_Id\" limit $1) ORDER BY \"Order\".\"Order_Id\" LIMIT 9;", 1, NULL, (const char*[]){prelude_entrys}, NULL, NULL, 0);

	PGresult* res_del = PQexecParams(db_conn, "CALL delete_cart($1);", 1, NULL, (const char*[]){PQgetvalue(res_orders, f, 0)}, NULL, NULL, 0);

	if(PQresultStatus(res_del) == PGRES_FATAL_ERROR){printf("FAILED TO DELETE ORDER %s\n", PQgetvalue(res_orders, f, 0));}

	return callback_3_empl(buf, n, db_conn, sockfd);


}

uint64_t callback_11_empl(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // next page order
	curpage_cart_empl++;
	return callback_3_empl(buf, n, db_conn, sockfd);
}

uint64_t callback_12_empl(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // prev page order
	curpage_cart_empl--;
	return callback_3_empl(buf, n, db_conn, sockfd);
}


uint64_t callback_13_empl(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // send message
	char* message = buf;

	char eid[20];
	sprintf(eid, "%ld", client_id_empl);

	char cid[20];
	sprintf(cid, "%ld", sel_chat);

	PGresult* res_chid = PQexecParams(db_conn, "SELECT \"Chat\".\"Chat_Id\" FROM \"Chat\" WHERE \"Chat\".\"Client_Id\" = $1;", 1, NULL, (const char*[]){cid}, NULL, NULL, 0);

	printf("CHAT ID: %s, MSG: %s\n", PQgetvalue(res_chid, 0, 0), message);

	PGresult* res_send = PQexecParams(db_conn, "INSERT INTO \"Message\"(\"Text\", \"Employee_id\", \"Chat_Id\") VALUES($1, $2, $3);", 3, NULL, (const char*[]){message, eid, PQgetvalue(res_chid, 0, 0)}, NULL, NULL, 0);
	if(PQresultStatus(res_send) == PGRES_FATAL_ERROR){
		printf("Failed to send Message!\n");
	}

	return calb_6_ghost(buf, n, db_conn, sockfd);
}

uint64_t callback_14_empl(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // up button
	curpage_chat_empl--;
	printf("CURPG: %ld\n", curpage_chat_empl);
	return calb_6_ghost(buf, n, db_conn, sockfd);
}

uint64_t callback_15_empl(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // down button
	curpage_chat_empl++;
	printf("CURPG: %ld\n", curpage_chat_empl);
	return calb_6_ghost(buf, n, db_conn, sockfd);
}



uint64_t (*callbacks_empl[30])(void*, uint64_t, PGconn*, int) = {callback_0_empl, callback_1_empl, callback_2_empl, callback_3_empl, callback_4_empl, callback_5_empl, callback_6_empl, callback_7_empl, callback_8_empl, callback_9_empl, callback_10_empl, callback_11_empl, callback_12_empl, callback_13_empl, callback_14_empl, callback_15_empl};