#include <unistd.h>
#include <stdlib.h>
#include <string.h>

extern void* windows[20];

struct button{
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

struct text{
	float r;
	float g;
	float b;
	float x;
	float y;
	uint8_t font_ptr;
}__attribute__((packed));

struct box{
	float r;
	float g;
	float b;
	float x;
	float y;
	float length;
	float heigth;
}__attribute__((packed));


uint64_t client_id = 0xFFFFFFFFFFFFFFFF;
uint64_t curpage = 0;
uint64_t curpage_cart = 0;
uint64_t curpage_chat = 0;

uint64_t is_numeric(char* s){ while(*s != '\0'){ if(*s < '0' || *s > '9'){ return 0;}}return 1;}


uint64_t callback_0(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // exit callback
	printf("Disconnected!\n");
	_exit(0);
}

uint64_t callback_1(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // register callback
	return 1;
}

uint64_t callback_2(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // log in callback
	return 3;
}

uint64_t callback_3(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // apply button in register window
	//printf("%s %s\n", buf, buf+strlen(buf)+1);
	//printf("Callbackid: %ld, ", *((uint64_t*)buf));
	//char* ptr = buf+8;


	char* client_name = buf;
	char* client_date_of_birth = client_name+strlen(client_name)+1;
	char* client_phone = client_date_of_birth+strlen(client_date_of_birth)+1;
	//char* client_card = client_phone+strlen(client_phone)+1;
	//char* client_cvc = client_card+strlen(client_card)+1;
	char* client_login = client_phone+strlen(client_phone)+1;
	char* client_password = client_login+strlen(client_login)+1;

	//const char* params[5] = {client_name, client_date_of_birth, client_phone, client_login, client_password};

	/*const char* query = 
	"INSERT INTO s333219.Client (Name, Date_Of_Birth, Phone_Number, Payment_Way_Id, Login_Data_Id) "
	"VALUES ($1, $2, $3, )";*/

	printf("Client name: %s\n", client_name);
	printf("date of birth: %s\n", client_date_of_birth);
	printf("Phone: %s\n", client_phone);
	//printf("Card: %s\n", client_card);
	//printf("CVC: %s\n", client_cvc);
	printf("Username: %s\n", client_login);
	printf("Password: %s\n", client_password);

	// need to insert a new client (name, date, uname, pwd). First need the check validness of phone and date. 

	// validness checks






	const char* params_1[2] = {client_login, client_password};
	PGresult* res_1 = PQexecParams(db_conn, "INSERT INTO \"s333219\".\"Login\" (\"Login\", \"Password\") VALUES ($1, $2);", 2, NULL, params_1, NULL, NULL, 0);
	if(PQresultStatus(res_1) == PGRES_FATAL_ERROR){return 2;}

	res_1 = PQexec(db_conn, "SELECT LASTVAL();");

	const char* params_2[4] = {client_name, client_date_of_birth, client_phone, PQgetvalue(res_1, 0, 0)};
	PGresult* res_2 = PQexecParams(db_conn, "INSERT INTO \"s333219\".\"Client\" (\"Name\", \"Date_Of_Birth\", \"Phone_Number\", \"Login_Data_Id\") VALUES ($1, $2, $3, $4);", 4, NULL, params_2, NULL, NULL, 0);

	if(PQresultStatus(res_2) == PGRES_FATAL_ERROR){
		const char* params_3[1] = {PQgetvalue(res_1, 0, 0)};
		res_2 = PQexecParams(db_conn, "DELETE FROM \"s333219\".\"Login\" WHERE \"Login\".\"Pass_Id\" = $1;", 1, NULL, params_3, NULL, NULL, 0);
		return 2;
	}

	res_1 = PQexec(db_conn, "SELECT LASTVAL();");

	res_2 = PQexecParams(db_conn, "INSERT INTO \"Chat\"(\"Client_Id\") VALUES($1);", 1, NULL, (const char*[]){PQgetvalue(res_1, 0, 0)}, NULL, NULL, 0);

	client_id = atoll(PQgetvalue(res_1, 0, 0));

	printf("USER %ld REGISTERED\n", client_id);

	// PGresult* res = PQexec(db_conn, "SELECT last_value FROM \"s333219\".\"Login_Pass_Id_seq\";"); // get the last id




	// if(PQresultStatus(res) == PGRES_TUPLES_OK){
    //     printf("QUERY GOOD\n");
	// }else{
	// 	printf("QUERY FAILED\n");
	// }


	//printf("%s %s\n", PQfname(res, 0), PQgetvalue(res, 0, 0));
	

	return 5;
}

uint64_t callback_4(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // apply button in log in
	char* username = buf;
	char* password = username + strlen(username) + 1;

	printf("LOGIN ATTEMPT: %s %s\n", username, password);

	PGresult* res = PQexecParams(db_conn, "SELECT \"Client\".\"Client_Id\" FROM \"Client\" JOIN \"Login\" ON \"Client\".\"Login_Data_Id\" = \"Login\".\"Pass_Id\" WHERE \"Login\".\"Login\" = $1 AND \"Login\".\"Password\" = $2;", 2, NULL, (const char*[]){username, password}, NULL, NULL, 0);

	if(PQresultStatus(res)==PGRES_FATAL_ERROR){return 4;}

	if(!PQntuples(res)){return 4;}

	client_id = atoll(PQgetvalue(res, 0, 0));

	printf("USER %ld LOGGED IN\n", client_id);

	return 5;

}

uint64_t callback_5(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // catalogue button on main page
	// dynamically form a window 

	//printf("ENTERED CALLBACK5\n");

	PGresult* n_pages = PQexec(db_conn, "SELECT COUNT(*) FROM \"Beer\";");
		
	char prelude_pages[20];
	sprintf(prelude_pages, "%ld", curpage*9);

	//printf("%s BEERS\n", PQgetvalue(n_pages, 0, 0));

	PGresult* res = PQexecParams(db_conn, "SELECT \"Beer\".\"Beer_Name\" FROM \"Beer\" WHERE \"Beer\".\"Beer_Id\" NOT IN (SELECT \"Beer\".\"Beer_Id\" FROM \"Beer\" ORDER BY \"Beer\".\"Beer_Id\" limit $1) ORDER BY \"Beer\".\"Beer_Id\" LIMIT 9;", 1, NULL, (const char*[]){prelude_pages}, NULL, NULL, 0);

	uint64_t n_beers = PQntuples(res);

	//printf("BEERS TO PRINT %ld\n", n_beers);

	//printf("STUFF TO WRITE %ld\n", *((uint64_t*)windows[9])-8-31);

	memcpy(buf, windows[9]+8, *((uint64_t*)windows[9])-8-31);
	void* ptr = buf+*((uint64_t*)windows[9])-8-31;

	//printf("1!!!\n");

	float yi = 0.9f;
	for(uint64_t i = 0; i < n_beers; i++){
		*((struct button*)ptr) = (struct button){r:1.0f, g:1.0f, b:0.0f, x:-0.3f, y:yi, tr:0.0f, tg:0.0f, tb:0.0f, length:0.6f, heigth:0.1f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:6, button_r_type:12};
		yi -= 0.2f;
		ptr += sizeof(struct button);
		(*((uint8_t*)buf+1))++;
	}

	if(curpage){
		*((struct button*)ptr) = (struct button){r:0.0f, g:0.0f, b:1.0f, x:-0.2f, y:-0.9f, tr:0.0f, tg:0.0f, tb:0.0f, length:0.1f, heigth:0.1f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:6, button_r_type:13};
		ptr += sizeof(struct button);
		(*((uint8_t*)buf+1))++;
	}
	if(curpage+1 != atoll(PQgetvalue(n_pages, 0, 0))/9 + !(atoll(PQgetvalue(n_pages, 0, 0))%9 == 0)){
		*((struct button*)ptr) = (struct button){r:0.0f, g:0.0f, b:1.0f, x:-0.0f, y:-0.9f, tr:0.0f, tg:0.0f, tb:0.0f, length:0.1f, heigth:0.1f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:6, button_r_type:14};
		ptr += sizeof(struct button);		
		(*((uint8_t*)buf+1))++;
	}


	//printf("2!!!\n");

	memcpy(ptr, windows[9]+8+*((uint64_t*)windows[9])-8-31, 10);
	ptr += 10;

	for(uint64_t i = 0; i < n_beers; i++){
		char* str = PQgetvalue(res, i, 0);
		uint64_t len = strlen(str);
		memcpy(ptr, str, len+1);
		ptr += len+1;
	}

	if(curpage){
		memcpy(ptr, "<-", 3);
		ptr += 3;
	}
	if(curpage+1 != atoll(PQgetvalue(n_pages, 0, 0))/9 + !(atoll(PQgetvalue(n_pages, 0, 0))%9 == 0)){
		memcpy(ptr, "->", 3);
		ptr += 3;
	}

	//printf("3!!!\n");

	memcpy(ptr, windows[9]+8+*((uint64_t*)windows[9])-8-21, 21);
	ptr += 21;

	memcpy(ptr, "Page ", 5);
	ptr += 5;

	char number[20];
	sprintf(number, "%ld", curpage+1);
	memcpy(ptr, number, strlen(number));
	ptr += strlen(number);

	memcpy(ptr, " of ", 4);
	ptr += 4;

	sprintf(number, "%ld", atoll(PQgetvalue(n_pages, 0, 0))/9 + !(atoll(PQgetvalue(n_pages, 0, 0))%9 == 0));
	memcpy(ptr, number, strlen(number)+1);
	ptr += strlen(number)+1;

	send(sockfd, buf, ptr-buf, 0);

	return -1;

}

uint64_t callback_6(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // cart button on main page
	memcpy(buf, windows[13]+8, *((uint64_t*)windows[13])-8-31);
	void* ptr = buf + *((uint64_t*)windows[13])-8-31;

	char cid[20];
	sprintf(cid, "%ld", client_id);

	PGresult* n_entrys_res = PQexecParams(db_conn, "SELECT COUNT(*) FROM \"Shopping_Cart\" WHERE \"Shopping_Cart\".\"Client_Id\" = $1;", 1, NULL, (const char*[]){cid}, NULL, NULL, 0);
	uint64_t total_n = atoll(PQgetvalue(n_entrys_res, 0, 0));
	//printf("NENTRYS %ld of USER %ld\n", total_n, client_id);

	char prelude_pages[20];
	sprintf(prelude_pages, "%ld", curpage_cart*9);

	PGresult* entrys = PQexecParams(db_conn, "SELECT \"Shopping_Cart\".\"Order_Id\" FROM \"Shopping_Cart\" WHERE ( \"Shopping_Cart\".\"Order_Id\" NOT IN (SELECT \"Shopping_Cart\".\"Order_Id\" FROM \"Shopping_Cart\" WHERE \"Shopping_Cart\".\"Client_Id\" = $1 ORDER BY \"Shopping_Cart\".\"Order_Id\" LIMIT $2)  AND  \"Shopping_Cart\".\"Client_Id\" = $1) ORDER BY \"Order_Id\" LIMIT 9;", 2, NULL, (const char*[]){cid, prelude_pages}, NULL, NULL, 0);
	uint64_t n_entrys = PQntuples(entrys);

	//printf("NENTRYS %ld\n", n_entrys);

	if(!n_entrys){
		//printf("NO ENTRYS\n");
		memcpy(ptr, windows[13]+8+*((uint64_t*)windows[13])-8-31, 31);
		ptr += 31;
		*((struct text*)ptr) = (struct text){r:0.0, g:0.0, b:0.0, x:-0.1, y:0.0, font_ptr:3};
		(*((uint8_t*)(buf+2)))++;
		ptr += sizeof(struct text);
		memcpy(ptr, "Your cart:\0Your cart is empty!", 31);
		ptr += 31;
		send(sockfd, buf, ptr-buf, 0);
		return -1;
	}
//forth | stack | neum | hw | instr | binary | trap | mem | pstr | prob1 | spi
	float yi = 0.9f;

	for(uint64_t i = 0; i < n_entrys; i++){
		PGresult* res_sid = PQexecParams(db_conn, "SELECT \"Order\".\"Status_Id\" FROM \"Order\" WHERE \"Order\".\"Order_Id\" = $1;", 1, NULL, (const char*[]){PQgetvalue(entrys, i, 0)}, NULL, NULL, 0);
		if(atoll(PQgetvalue(res_sid, 0, 0)) == 6){
			*((struct button*)ptr) = (struct button){r:0.0f, g:1.0f, b:0.0f, x:0.65f, y:yi, tr:0.0f, tg:0.0f, tb:0.0f, length:0.2f, heigth:0.1f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:3, button_r_type:16}; // confirm order
			ptr += sizeof(struct button);
			(*((uint8_t*)buf+1))++;
		}
		*((struct button*)ptr) = (struct button){r:1.0f, g:0.0f, b:0.0f, x:0.85f, y:yi, tr:0.0f, tg:0.0f, tb:0.0f, length:0.15f, heigth:0.1f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:3, button_r_type:17};     // delete order
		ptr += sizeof(struct button);
		(*((uint8_t*)buf+1))++;

		yi -= 0.2f;
	}

	if(curpage_cart){
		*((struct button*)ptr) = (struct button){r:0.0f, g:0.0f, b:1.0f, x:-0.2f, y:-0.9f, tr:0.0f, tg:0.0f, tb:0.0f, length:0.1f, heigth:0.1f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:6, button_r_type:18};  // prev button
		ptr += sizeof(struct button);
		(*((uint8_t*)buf+1))++;
	}

	if(curpage_cart+1 != total_n/9 + !(total_n%9 == 0)){
		*((struct button*)ptr) = (struct button){r:0.0f, g:0.0f, b:1.0f, x:-0.0f, y:-0.9f, tr:0.0f, tg:0.0f, tb:0.0f, length:0.1f, heigth:0.1f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:6, button_r_type:19};  // next button
		ptr += sizeof(struct button);		
		(*((uint8_t*)buf+1))++;
	}

	memcpy(ptr, windows[13]+8+*((uint64_t*)windows[13])-8-31, 10);
	ptr += 10;

	for(uint64_t i = 0; i < n_entrys; i++){
		PGresult* res_sid = PQexecParams(db_conn, "SELECT \"Order\".\"Status_Id\" FROM \"Order\" WHERE \"Order\".\"Order_Id\" = $1;", 1, NULL, (const char*[]){PQgetvalue(entrys, i, 0)}, NULL, NULL, 0);
		if(atoll(PQgetvalue(res_sid, 0, 0)) == 6){
			memcpy(ptr, "Confirm", 8);
			ptr += 8;
		}
		memcpy(ptr, "Delete", 7);
		ptr += 7;
	}

	if(curpage_cart){
		memcpy(ptr, "<-", 3);
		ptr += 3;
	}
	if(curpage_cart+1 != total_n/9 + !(total_n%9 == 0)){
		memcpy(ptr, "->", 3);
		ptr += 3;
	}

	yi = 0.9f;
	for(uint64_t i = 0; i < n_entrys; i++){
		*((struct box*)ptr) = (struct box){r:1.0f, g:1.0f, b:0.0f, x:-0.9f, y:yi, length:1.7f, heigth:0.1f};
		ptr += sizeof(struct box);
		(*((uint8_t*)(buf+3)))++;	
		yi -= 0.2f;
	}


	memcpy(ptr, windows[13]+8+*((uint64_t*)windows[13])-8-21, 21);
	ptr += 21;

	yi = 0.9f;
	for(uint64_t i = 0; i < n_entrys; i++){
		*((struct text*)ptr) = (struct text){r:0.0f, g:0.0f, b:0.0f, x:-0.9f, y:yi-0.05f, font_ptr:6};
		ptr += sizeof(struct text);
		yi -= 0.2f;
	}

	char number[20];
	memcpy(ptr, "Your cart. Page ", 16);
	ptr += 16;

	sprintf(number, "%ld", curpage_cart+1);
	memcpy(ptr, number, strlen(number));
	ptr += strlen(number);

	memcpy(ptr, " of ", 4);
	ptr += 4;

	sprintf(number, "%ld", total_n/9 + !(total_n%9 == 0));
	memcpy(ptr, number, strlen(number)+1);
	ptr += strlen(number)+1;

	for(uint64_t i = 0; i < n_entrys; i++){
		memcpy(ptr, "Beer: ", 6);
		ptr += 6;
		PGresult* amount_bid_res = PQexecParams(db_conn, "SELECT \"Amount_Of_Beer\".\"Amount_Of_Beer\", \"Amount_Of_Beer\".\"Beer_Id\" FROM \"Amount_Of_Beer\" WHERE \"Amount_Of_Beer\".\"Order_Id\" = $1;", 1, NULL, (const char*[]){PQgetvalue(entrys, i, 0)}, NULL, NULL, 0);
		PGresult* bname_res = PQexecParams(db_conn, "SELECT \"Beer\".\"Beer_Name\" FROM \"Beer\" WHERE \"Beer\".\"Beer_Id\" = $1;", 1, NULL, (const char*[]){PQgetvalue(amount_bid_res, 0, 1)}, NULL, NULL, 0);
		char* name = PQgetvalue(bname_res, 0, 0);
		uint64_t namelen = strlen(name);
		memcpy(ptr, name, namelen);
		ptr += namelen;
		memcpy(ptr, " Amount: ", 9);
		ptr += 9;
		char* amount = PQgetvalue(amount_bid_res, 0, 0);
		uint64_t amountlen = strlen(amount);
		memcpy(ptr, amount, amountlen);
		ptr += amountlen;
		memcpy(ptr, " Order status: ", 15);
		ptr += 15;
		PGresult* status_res = PQexecParams(db_conn, "SELECT \"Status\".\"Current_Status\" FROM \"Status\" WHERE \"Status\".\"Status_Id\" = (SELECT \"Order\".\"Status_Id\" FROM \"Order\" WHERE \"Order\".\"Order_Id\" = $1);", 1, NULL, (const char*[]){PQgetvalue(entrys, i, 0)}, NULL, NULL, 0);
		char* status = PQgetvalue(status_res, 0, 0);
		uint64_t statuslen = strlen(status);
		memcpy(ptr, status, statuslen+1);
		ptr += statuslen+1;
		(*((uint8_t*)(buf+2)))++;
	}



	send(sockfd, buf, ptr-buf, 0);
	return -1;

}

uint64_t callback_7(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // profile button on main page
	char id[20];
	sprintf(id, "%ld", client_id);

	memcpy(buf, windows[6]+8, *((uint64_t*)windows[6])-8);

	PGresult* res = PQexecParams(db_conn, "SELECT * FROM \"Client\" WHERE \"Client\".\"Client_Id\" = $1;", 1, NULL, (const char*[]){id}, NULL, NULL, 0);
	if(PQresultStatus(res) == PGRES_FATAL_ERROR){return 5;}

	char* lid = PQgetvalue(res, 0, 5);
	PGresult* resl = PQexecParams(db_conn, "SELECT * FROM \"Login\" WHERE \"Login\".\"Pass_Id\" = $1;", 1, NULL, (const char*[]){lid}, NULL, NULL, 0);

	char* pid = PQgetvalue(res, 0, 4);
	PGresult* resp;
	if(strlen(pid)){
		resp = PQexecParams(db_conn, "SELECT * FROM \"Online_Pay\" WHERE \"Online_Pay\".\"Pay_Id\" = $1;", 1, NULL, (const char*[]){pid}, NULL, NULL, 0);
	}

	void* ptr = buf + *((uint64_t*)windows[6])-8;

	memcpy(ptr, PQgetvalue(resl, 0, 1), strlen(PQgetvalue(resl, 0, 1))+1);
	ptr += strlen(PQgetvalue(resl, 0, 1))+1; // uname

	memcpy(ptr, PQgetvalue(resl, 0, 2), strlen(PQgetvalue(resl, 0, 2))+1);
	ptr += strlen(PQgetvalue(resl, 0, 2))+1; // pwd

	memcpy(ptr, PQgetvalue(res, 0, 1), strlen(PQgetvalue(res, 0, 1))+1);
	ptr += strlen(PQgetvalue(res, 0, 1))+1; // nm

	memcpy(ptr, PQgetvalue(res, 0, 2), strlen(PQgetvalue(res, 0, 2))+1);
	ptr += strlen(PQgetvalue(res, 0, 2))+1; // dt

	memcpy(ptr, PQgetvalue(res, 0, 3), strlen(PQgetvalue(res, 0, 3))+1);
	ptr += strlen(PQgetvalue(res, 0, 3))+1; // ph

	if(strlen(pid)){
		memcpy(ptr, PQgetvalue(resp, 0, 1), strlen(PQgetvalue(resp, 0, 1))+1);
		ptr += strlen(PQgetvalue(resp, 0, 1))+1; // crd		
		memcpy(ptr, PQgetvalue(resp, 0, 2), strlen(PQgetvalue(resp, 0, 2))+1);
		ptr += strlen(PQgetvalue(resp, 0, 2))+1; // cvc
	}else{
		const char* nth = "nothing";
		memcpy(ptr, nth, 8);
		ptr += 8; // crd
		memcpy(ptr, nth, 8);
		ptr += 8; // cvc
	}

	send(sockfd, buf, ptr-buf, 0);

	return -1;
}

uint64_t callback_8(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // back button in profile
	curpage = 0;
	curpage_cart = 0;
	curpage_chat = 0;
	return 5;
}

uint64_t callback_9(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // edit button in profile
	return 7;
}

uint64_t callback_10(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // back button in edit cred 
	return callback_7(buf, n, db_conn, sockfd);
}

uint64_t callback_11(void* buf, uint64_t n, PGconn* db_conn, int sockfd){
	char cid[20];
	sprintf(cid, "%ld", client_id);

	char* card = buf;
	char* cvc = card + strlen(card) + 1;

	if(strlen(cvc) != 3){
		return 8;
	}
	if(strlen(card) != 16){
		return 8;
	}


	printf("Edit cred: %s %s\n", card, cvc);


	PGresult* resp = PQexecParams(db_conn, "SELECT \"Client\".\"Payment_Way_Id\" FROM \"Client\" WHERE \"Client\".\"Client_Id\" = $1;", 1, NULL, (const char*[]){cid}, NULL, NULL, 0);
	if(PQresultStatus(resp) == PGRES_FATAL_ERROR){
		new_cred:;

		PGresult* res = PQexecParams(db_conn, "INSERT INTO \"Online_Pay\"(\"Card_Number\", \"CVC\") VALUES($1, $2);", 2, NULL, (const char*[]){card, cvc}, NULL, NULL, 0);

		if(PQresultStatus(res) == PGRES_FATAL_ERROR){return 7;}
		res = PQexec(db_conn, "SELECT LASTVAL();");
		printf("!!!!!!!%s\n", PQgetvalue(res, 0, 0));
		PGresult* res_2 = PQexecParams(db_conn, "UPDATE \"Client\" SET \"Payment_Way_Id\" = $1 WHERE \"Client\".\"Client_Id\" = $2;", 2, NULL, (const char*[]){PQgetvalue(res, 0, 0), cid}, NULL, NULL, 0);
		if(PQresultStatus(res_2)==PGRES_FATAL_ERROR){printf("FATAL ERROR!!!\n");}
		return callback_7(buf, n, db_conn, sockfd);			
	}
	if(!strlen(PQgetvalue(resp, 0, 0))){goto new_cred;}

	PGresult* res = PQexecParams(db_conn, "UPDATE \"Online_Pay\" SET \"Card_Number\" = $1, \"CVC\" = $2 where \"Online_Pay\".\"Pay_Id\" = $3;", 3, NULL, (const char*[]){card, cvc, PQgetvalue(resp, 0, 0)}, NULL, NULL, 0);

	return callback_7(buf, n, db_conn, sockfd);	
}

uint64_t callback_12(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // beer button in catalogue
	PGresult* res = PQexecParams(db_conn, "SELECT \"Beer\".\"Beer_Id\" FROM \"Beer\" WHERE \"Beer\".\"Beer_Name\" = $1;", 1, NULL, (const char*[]){buf}, NULL, NULL, 0);
	if(PQntuples(res)){
		memcpy(buf+strlen(buf)+1, windows[10]+8, *((uint64_t*)windows[10])-8);
		memcpy(buf+strlen(buf)+1+*((uint64_t*)windows[10])-8, buf, strlen(buf)+1);
		send(sockfd, buf+strlen(buf)+1, strlen(buf)+1+*((uint64_t*)windows[10])-8, 0);
		return -1;
	}else{
		return 0;
	}
}

uint64_t callback_13(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // prev button in catalogue
	curpage--;
	return callback_5(buf, n, db_conn, sockfd);
}


uint64_t callback_14(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // next button in catalogue
	curpage++;
	return callback_5(buf, n, db_conn, sockfd);
}

uint64_t callback_15(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // apply button in beer profile
	char* beername = buf;
	uint64_t amount = atoll(beername+strlen(beername)+1);
	if(!amount){
		send_error_win:;
		memcpy(buf+strlen(buf)+1, windows[11]+8, *((uint64_t*)windows[11])-8);
		memcpy(buf+strlen(buf)+1+*((uint64_t*)windows[11])-8, buf, strlen(buf)+1);
		send(sockfd, buf+strlen(buf)+1, strlen(buf)+1+*((uint64_t*)windows[11])-8, 0);
		return -1;		
		// send error window
	}

	PGresult* res_beer = PQexecParams(db_conn, "SELECT \"Beer\".\"Beer_Id\" FROM \"Beer\" WHERE \"Beer\".\"Beer_Name\" = $1;", 1, NULL, (const char*[]){beername}, NULL, NULL, 0);
	if(!PQntuples(res_beer)){goto send_error_win;}


	//PGresult* res_status = PQexec(db_conn, "INSERT INTO \"Status\"(\"Current_Status\") VALUES('Not Confirmed by the Client');");
	//res_status = PQexec(db_conn, "SELECT LASTVAL();");


	PGresult* res_order = PQexecParams(db_conn, "INSERT INTO \"Order\"(\"Status_Id\") VALUES($1);", 1, NULL, (const char*[]){"6"/*PQgetvalue(res_status, 0, 0)*/}, NULL, NULL, 0);
	res_order = PQexec(db_conn, "SELECT LASTVAL();");

	PGresult* res_amobr = PQexecParams(db_conn, "INSERT INTO \"Amount_Of_Beer\"(\"Order_Id\", \"Beer_Id\", \"Amount_Of_Beer\") VALUES($1, $2, $3);", 3, NULL, (const char*[]){PQgetvalue(res_order, 0, 0), PQgetvalue(res_beer, 0, 0), buf+strlen(beername)+1}, NULL, NULL, 0);

	char cid[20];
	sprintf(cid, "%ld", client_id);

	PGresult* res_cart = PQexecParams(db_conn, "INSERT INTO \"Shopping_Cart\"(\"Client_Id\", \"Order_Id\") VALUES($1, $2);", 2, NULL, (const char*[]){cid, PQgetvalue(res_order, 0, 0)}, NULL, NULL, 0);

	// send success window
	memcpy(buf+strlen(buf)+1, windows[12]+8, *((uint64_t*)windows[12])-8);
	memcpy(buf+strlen(buf)+1+*((uint64_t*)windows[12])-8, buf, strlen(buf)+1);
	send(sockfd, buf+strlen(buf)+1, strlen(buf)+1+*((uint64_t*)windows[12])-8, 0);
	return -1;

}

uint64_t callback_16(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // confirm button in cart
	uint64_t f = atoll(buf);

	char prelude_entrys[20];
	sprintf(prelude_entrys, "%ld", curpage_cart*9);

	char cid[20];
	sprintf(cid, "%ld", client_id);

	PGresult* res_ordid = PQexecParams(db_conn, "SELECT \"Shopping_Cart\".\"Order_Id\" FROM \"Shopping_Cart\" WHERE ( \"Shopping_Cart\".\"Order_Id\" NOT IN (SELECT \"Shopping_Cart\".\"Order_Id\" FROM \"Shopping_Cart\" WHERE \"Shopping_Cart\".\"Client_Id\" = $1 ORDER BY \"Shopping_Cart\".\"Order_Id\" LIMIT $2)  AND  \"Shopping_Cart\".\"Client_Id\" = $1) ORDER BY \"Order_Id\" LIMIT 9;", 2, NULL, (const char*[]){cid, prelude_entrys}, NULL, NULL, 0);

	if(PQresultStatus(res_ordid) == PGRES_FATAL_ERROR){goto end;}
	if(!PQntuples(res_ordid)){goto end;}

	//printf("FIRST CHECK PASSED\n");

	PGresult* res_stsid = PQexecParams(db_conn, "SELECT \"Order\".\"Status_Id\" FROM \"Order\" WHERE \"Order\".\"Order_Id\" = $1;", 1, NULL, (const char*[]){PQgetvalue(res_ordid, f, 0)}, NULL, NULL, 0);

	if(PQresultStatus(res_stsid) == PGRES_FATAL_ERROR){goto end;}
	if(!PQntuples(res_stsid)){goto end;}	
	if(atoll(PQgetvalue(res_stsid, 0, 0)) != 6){goto end;}

	printf("SECOND CHECK PASSED\n");

	PGresult* res_supdt = PQexecParams(db_conn, "UPDATE \"Order\" SET \"Status_Id\" = 5 WHERE \"Order\".\"Order_Id\" = $1;", 1, NULL, (const char*[]){PQgetvalue(res_ordid, f, 0)}, NULL, NULL, 0);

	end:;
	return callback_6(buf, n, db_conn, sockfd);
}

uint64_t callback_17(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // delete button in cart
	uint64_t f = atoll(buf);

	char prelude_entrys[20];
	sprintf(prelude_entrys, "%ld", curpage_cart*9);

	char cid[20];
	sprintf(cid, "%ld", client_id);

	PGresult* res_ordid = PQexecParams(db_conn, "SELECT \"Shopping_Cart\".\"Order_Id\" FROM \"Shopping_Cart\" WHERE ( \"Shopping_Cart\".\"Order_Id\" NOT IN (SELECT \"Shopping_Cart\".\"Order_Id\" FROM \"Shopping_Cart\" WHERE \"Shopping_Cart\".\"Client_Id\" = $1 ORDER BY \"Shopping_Cart\".\"Order_Id\" LIMIT $2)  AND  \"Shopping_Cart\".\"Client_Id\" = $1) ORDER BY \"Order_Id\" LIMIT 9;", 2, NULL, (const char*[]){cid, prelude_entrys}, NULL, NULL, 0);

	if(PQresultStatus(res_ordid) == PGRES_FATAL_ERROR){goto end;}
	if(!PQntuples(res_ordid)){goto end;}

	PGresult* del = PQexecParams(db_conn, "CALL delete_cart($1);", 1, NULL, (const char*[]){PQgetvalue(res_ordid, f, 0)}, NULL, NULL, 0);


	end:;
	return callback_6(buf, n, db_conn, sockfd);
}

uint64_t callback_18(void* buf, uint64_t n, PGconn* db_conn, int sockfd){
	curpage_cart--;
	return callback_6(buf, n, db_conn, sockfd);
}

uint64_t callback_19(void* buf, uint64_t n, PGconn* db_conn, int sockfd){
	curpage_cart++;
	return callback_6(buf, n, db_conn, sockfd);
}


uint64_t callback_20(void* buf, uint64_t n, PGconn* db_conn, int sockfd){
	char cid[20];
	sprintf(cid, "%ld", client_id);

	printf("DELETING CLIENT %s\n", cid);

	PGresult* res =  PQexecParams(db_conn, "CALL delete_client($1);", 1, NULL, (const char*[]){cid}, NULL, NULL, 0);

	if(PQresultStatus(res) == PGRES_FATAL_ERROR){
		printf("FAILED TO DELETE\n");
	}
	if(PQresultStatus(res) == PGRES_COMMAND_OK){
		printf("SUCCESSFULLY DELETED\n");
	}

	client_id = 0;
	curpage = 0;
	curpage_cart = 0;
	curpage_chat = 0;

	return 0;
}

uint64_t callback_21(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // the chat button
	memcpy(buf, windows[14]+8, *((uint64_t*)windows[14])-8-15);

	char cid[20];
	sprintf(cid, "%ld", client_id);

	PGresult* res_chid = PQexecParams(db_conn, "SELECT \"Chat\".\"Chat_Id\" FROM \"Chat\" WHERE \"Chat\".\"Client_Id\" = $1;", 1, NULL, (const char*[]){cid}, NULL, NULL, 0);
	if(!PQntuples(res_chid)){
		PQexecParams(db_conn, "INSERT INTO \"Chat\" (\"Client_Id\") VALUES($1);", 1, NULL, (const char*[]){cid}, NULL, NULL, 0);
	}

	char prelude_entrys[20];
	sprintf(prelude_entrys, "%ld", curpage_chat*9);

	PGresult* res_mesg = PQexecParams(db_conn, "SELECT \"Text\", \"Employee_id\" FROM \"Message\" WHERE \"Message\".\"Message_Id\" NOT IN (SELECT \"Message\".\"Message_Id\" FROM \"Message\" WHERE \"Chat_Id\" = $1 ORDER BY \"Message\".\"Message_Id\" DESC limit $2) AND \"Chat_Id\" = $1 ORDER BY \"Message\".\"Message_Id\" DESC limit 9;", 2, NULL, (const char*[]){PQgetvalue(res_chid, 0, 0), prelude_entrys}, NULL, NULL, 0);
	uint64_t n_mesg = PQntuples(res_mesg);

	PGresult* res_totl = PQexecParams(db_conn, "SELECT COUNT(*) FROM \"Message\" WHERE \"Message\".\"Chat_Id\" = $1;", 1, NULL, (const char*[]){PQgetvalue(res_chid, 0, 0)}, NULL, NULL, 0);
	uint64_t total_n = atoll(PQgetvalue(res_totl, 0, 0));

	void* ptr = buf+*((uint64_t*)windows[14])-8-15;

	if(curpage_chat){
		*((struct button*)ptr) = (struct button){r:0.0f, g:0.0f, b:1.0f, x:0.9f, y:0.0f, tr:0.0f, tg:0.0f, tb:0.0f, length:0.1f, heigth:0.1f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:6, button_r_type:23};  // prev button
		ptr += sizeof(struct button);
		(*((uint8_t*)buf+1))++;	
	}

	if((curpage_chat+1 != total_n/9 + !(total_n%9 == 0)) && total_n != 0) {
		*((struct button*)ptr) = (struct button){r:0.0f, g:0.0f, b:1.0f, x:0.9f, y:0.2f, tr:0.0f, tg:0.0f, tb:0.0f, length:0.1f, heigth:0.1f, sr:0.0f, sg:0.0f, sb:0.0f, font_ptr:6, button_r_type:24};  // prev button
		ptr += sizeof(struct button);
		(*((uint8_t*)buf+1))++;			
	}

	memcpy(ptr, windows[14]+*((uint64_t*)windows[14])-15, 15);
	ptr += 15;


	if(curpage_chat){
		memcpy(ptr, "\\/", 3); 
		ptr += 3;
	}

	if(curpage_chat+1 != total_n/9 + !(total_n%9 == 0) && total_n != 0){
		memcpy(ptr, "/\\", 3);
		ptr += 3;
	}

	float yi = -0.7f;
	for(uint64_t i = 0; i < n_mesg; i++){
		if(atoll(PQgetvalue(res_mesg, i, 1))){ // sent by employee
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

uint64_t callback_22(void* buf, uint64_t n, PGconn* db_conn, int sockfd){ // send message
	char* message = buf;

	char cid[20];
	sprintf(cid, "%ld", client_id);
	PGresult* res_chid = PQexecParams(db_conn, "SELECT \"Chat\".\"Chat_Id\" FROM \"Chat\" WHERE \"Chat\".\"Client_Id\" = $1;", 1, NULL, (const char*[]){cid}, NULL, NULL, 0);

	printf("CHAT ID: %s, MSG: %s\n", PQgetvalue(res_chid, 0, 0), message);

	PGresult* res_send = PQexecParams(db_conn, "INSERT INTO \"Message\"(\"Text\", \"Employee_id\", \"Chat_Id\") VALUES($1, 0, $2);", 2, NULL, (const char*[]){message, PQgetvalue(res_chid, 0, 0)}, NULL, NULL, 0);
	if(PQresultStatus(res_send) == PGRES_FATAL_ERROR){
		printf("Failed to send Message!\n");
	}


	return callback_21(buf, n, db_conn, sockfd);
}

uint64_t callback_23(void* buf, uint64_t n, PGconn* db_conn, int sockfd){
	curpage_chat--;
	printf("CURPAGE: %ld\n", curpage_chat);
	return callback_21(buf, n, db_conn, sockfd);
}

uint64_t callback_24(void* buf, uint64_t n, PGconn* db_conn, int sockfd){
	curpage_chat++;
	printf("CURPAGE: %ld\n", curpage_chat);
	return callback_21(buf, n, db_conn, sockfd);
}


uint64_t (*callbacks[30])(void*, uint64_t, PGconn*, int) = {callback_0, callback_1, callback_2, callback_3, callback_4, callback_5, callback_6, callback_7, callback_8, callback_9, callback_10, callback_11, callback_12, callback_13, callback_14, callback_15, callback_16, callback_17, callback_18, callback_19, callback_20, callback_21, callback_22, callback_23, callback_24};




