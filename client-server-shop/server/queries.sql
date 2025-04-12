# check if username is present in table
SELECT exists (SELECT 1 FROM "s333219"."Login" WHERE "Login"."Login" = 'ChukhoninVan' LIMIT 1);

# insert new user
#first insert a "Login"


INSERT INTO "s333219"."Login" ("Login", "Password") VALUES ($1, $2)
SELECT last_value FROM "s333219"."Login_Pass_Id_seq" as lval
INSERT INTO "s333219"."Client" ("Name", "Date_Of_Birth", "Phone_Number", "Login_Data") VALUES ($1, $2, $3, lval);


#get client ID from username and password
SELECT "Client"."Client_Id" FROM "Client"
JOIN "Login" ON "Client"."Login_Data_Id" = "Login"."Pass_Id"
WHERE "Login"."Login" = 'Uname' AND "Login"."Password" = 'Pwd';


#get pay id from client
SELECT "Client"."Payment_Way_Id" FROM "Client" WHERE "Client"."Client_Id" = $1;


#update pay cred
UPDATE "Online_Pay" SET "Online_Pay"."Card_Number" = $1, "Online_Pay"."CVC" = $2 where "Online_Pay"."Pay_Id" = $3;

INSERT INTO "Online_Pay"("Card_Number", "CVC") VALUES($1, $2);
SELECT lastval();
UPDATE "Client" SET "Client"."Payment_Way_Id" = $1 WHERE "Client"."Client_Id" = $2;



#delete sample
#delete from "s333219"."Login" where "Login"."Pass_Id" = 4;



# get beer page
SELECT "Beer"."Beer_Name" FROM "Beer" WHERE "Beer"."Beer_Id" NOT IN (SELECT "Beer"."Beer_Id" FROM "Beer" ORDER BY "Beer"."Beer_Id" limit $1) ORDER BY "Beer"."Beer_Id" LIMIT 9;


# add order
INSERT INTO "Status"("Current_Status") VALUES("Not Confirmed by the Client");
SELECT LASTVAL();

INSERT INTO "Order"("Status_Id") VALUES($1);
SELECT LASTVAL();

INSERT INTO "Amount_Of_Beer"("Order_Id", "Beer_Id", "Amount_Of_Beer") VALUES($1, $2, $3);

INSERT INTO "Shopping_Cart"("Client_Id", "Order_Id") VALUES($1, $2, $3);


# get cart page



# get order ids
SELECT "Beer"."Beer_Name" FROM "Beer" WHERE "Beer"."Beer_Id" NOT IN (SELECT "Beer"."Beer_Id" FROM "Beer" ORDER BY "Beer"."Beer_Id" limit $1) ORDER BY "Beer"."Beer_Id" LIMIT 9;


#get amount of orders
SELECT COUNT(*) FROM "Shopping_Cart" WHERE "Shopping_Cart"."Client_Id" = $1;


# get order ids on page
SELECT "Shopping_Cart"."Order_Id" FROM "Shopping_Cart" WHERE ( "Shopping_Cart"."Order_Id" NOT IN (SELECT "Shopping_Cart"."Order_Id" FROM "Shopping_Cart" WHERE "Shopping_Cart"."Client_Id" = $1 ORDER BY "Shopping_Cart"."Order_Id" LIMIT $2)  AND  "Shopping_Cart"."Client_Id" = $1) ORDER BY "Order_Id" LIMIT 9;





SELECT "Shopping_Cart"."Order_Id" FROM "Shopping_Cart" WHERE "Shopping_Cart"."Client_Id" = $1;


#get status from order id
SELECT "Status"."Current_Status" FROM "Status" WHERE "Status"."Status_Id" = (SELECT "Order"."Status_Id" FROM "Order" WHERE "Order"."Order_Id" = $1);


#get amount of beer, beer id
SELECT "Amount_Of_Beer"."Amount_Of_Beer", "Amount_Of_Beer"."Beer_Id" FROM "Amount_Of_Beer" WHERE "Amount_Of_Beer"."Order_Id" = $1;

#get beer name from id
SELECT "Beer"."Beer_Name" FROM "Beer" WHERE "Beer"."Beer_Id" = $1;









select * from products
order by id
limit 10

select * from products
where id not in(select id from products oreder by id limit 10)
order by id
limit 10






UPDATE "Order" SET "Status_Id" = 5 WHERE "Order"."Order_Id" = $1;



INSERT INTO "Amount_Of_Beer"("Order_Id", "Beer_Id", "Amount_Of_Beer") VALUES(6, 2, 12);





# CLIENT CHAT

#get corresponding chat id
SELECT "Chat"."Chat_Id" FROM "Chat" WHERE "Chat"."Client_Id" = $1;

# get n of messages in chat
SELECT COUNT(*) FROM "Message" WHERE "Message"."Chat_Id" = $1;

#SELECT "Beer"."Beer_Name" FROM "Beer" WHERE "Beer"."Beer_Id" NOT IN (SELECT "Beer"."Beer_Id" FROM "Beer" ORDER BY "Beer"."Beer_Id" limit $1) ORDER BY "Beer"."Beer_Id" LIMIT 9;

# get messages on page
	

# get page of clients

SELECT "Client"."Name", "Client"."Date_Of_Birth", "Client"."Phone_Number", "Login"."Login", "Login"."Password" FROM "Client"
JOIN "Login" ON "Client"."Login_Data_Id" = "Login"."Pass_Id"
WHERE "Client"."Client_Id" NOT IN (SELECT "Client"."Client_Id" FROM "Client" ORDER BY "Client"."Client_Id" limit $1) ORDER BY "Client"."Client_Id" LIMIT 9;

# get page from all orders
SELECT "Order"."Order_Id" FROM "Order" WHERE "Order_Id" NOT IN (SELECT "Order"."Order_Id" FROM "Order" ORDER BY "Order"."Order_Id" limit $1) ORDER BY "Order"."Order_Id" LIMIT 9;