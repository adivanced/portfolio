#define IMPORT_BIN(sect, file, sym) asm (\
    ".section " #sect "\n"                  /* Change section */\
    ".balign 8\n"                           /* Word alignment */\
    ".global " #sym "\n"                    /* Export the object address */\
    #sym ":\n"                              /* Define the object label */\
    ".incbin \"" file "\"\n"                /* Import the file */\
    ".global _sizeof_" #sym "\n"            /* Export the object size */\
    ".set _sizeof_" #sym ", . - " #sym "\n" /* Define the object size */\
    ".balign 8\n"                           /* Word alignment */\
    ".section \".text\"\n")                 /* Restore section */


// usage: IMPORT_BIN(".rodata", "file.bin", nameassigned);

IMPORT_BIN(".rodata", "windows/bins/reg_log.bin", window_reg_log);
IMPORT_BIN(".rodata", "windows/bins/register.bin", window_register);
IMPORT_BIN(".rodata", "windows/bins/register_error.bin", window_register_error);
IMPORT_BIN(".rodata", "windows/bins/login.bin", window_login);
IMPORT_BIN(".rodata", "windows/bins/login_error.bin", window_login_error);
IMPORT_BIN(".rodata", "windows/bins/mainpg.bin", window_mainpg);
IMPORT_BIN(".rodata", "windows/bins/profile.bin", window_profile);
IMPORT_BIN(".rodata", "windows/bins/payedit.bin", window_payedit);
IMPORT_BIN(".rodata", "windows/bins/payedit_error.bin", window_payedit_error);
IMPORT_BIN(".rodata", "windows/bins/beer.bin", beer_page);
IMPORT_BIN(".rodata", "windows/bins/beer_profile.bin", beer_profile_page);
IMPORT_BIN(".rodata", "windows/bins/beer_profile_error.bin", beer_profile_error_page);
IMPORT_BIN(".rodata", "windows/bins/beer_profile_success.bin", beer_profile_success_page);
IMPORT_BIN(".rodata", "windows/bins/cart.bin", cart_page);
IMPORT_BIN(".rodata", "windows/bins/chat.bin", chat_page);

IMPORT_BIN(".rodata", "windows/employee/bins/login.bin", login_empl);
IMPORT_BIN(".rodata", "windows/employee/bins/login_error.bin", login_error_empl);
IMPORT_BIN(".rodata", "windows/employee/bins/mainpg.bin", mainpg_empl);
IMPORT_BIN(".rodata", "windows/employee/bins/clients.bin", clients_empl);
IMPORT_BIN(".rodata", "windows/employee/bins/chat.bin", chat_empl);

extern const char window_reg_log[];
extern const char window_register[];
extern const char window_register_error[];
extern const char window_login[];
extern const char window_login_error[];
extern const char window_mainpg[];
extern const char window_profile[];
extern const char window_payedit[];
extern const char window_payedit_error[];
extern const char beer_page[];
extern const char beer_profile_page[];
extern const char beer_profile_error_page[];
extern const char beer_profile_success_page[];
extern const char cart_page[];
extern const char chat_page[];
//extern const char _sizeof_window_reg_log[];


extern const char login_empl[];
extern const char login_error_empl[];
extern const char mainpg_empl[];
extern const char clients_empl[];
extern const char chat_empl[];

void* windows[20] = {(void*)window_reg_log, (void*)window_register, (void*)window_register_error, (void*)window_login, (void*)window_login_error, (void*)window_mainpg, (void*)window_profile, (void*)window_payedit, (void*)window_payedit_error, (void*)beer_page, (void*)beer_profile_page, (void*)beer_profile_error_page, (void*)beer_profile_success_page, (void*)cart_page, (void*)chat_page};

void* windows_empl[20] = {(void*)login_empl, (void*)login_error_empl, (void*)mainpg_empl, (void*)clients_empl, (void*)chat_empl};