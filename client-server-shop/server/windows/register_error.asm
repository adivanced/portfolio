sz dq totallen
window_packet_header:
	.n_text_fields db 5
	.n_buttons db 2
	.n_text db 7
	.n_boxes db 0
	.r dd 0.0
	.g dd 1.0
	.b dd 1.0
window_packet_text_fields:
	window_packet_text_field_name:
		.r dd 0.2
		.g dd 0.6
		.b dd 1.0
		.x dd -0.9
		.y dd 0.9
		.tr dd 1.0
		.tg dd 1.0
		.tb dd 1.0
		.length dd 0.8
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6	
	window_packet_text_field_date:
		.r dd 0.2
		.g dd 0.6
		.b dd 1.0
		.x dd -0.9
		.y dd 0.6
		.tr dd 1.0
		.tg dd 1.0
		.tb dd 1.0
		.length dd 0.4
		.height dd 0.1
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6	
	window_packet_text_field_phone:
		.r dd 0.2
		.g dd 0.6
		.b dd 1.0
		.x dd -0.9
		.y dd 0.4
		.tr dd 1.0
		.tg dd 1.0
		.tb dd 1.0
		.length dd 0.3
		.height dd 0.1
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6	
	; window_packet_text_field_card:
	; 	.r dd 0.2
	; 	.g dd 0.6
	; 	.b dd 1.0
	; 	.x dd -0.9
	; 	.y dd 0.1
	; 	.tr dd 1.0
	; 	.tg dd 1.0
	; 	.tb dd 1.0
	; 	.length dd 0.4
	; 	.height dd  0.1
	; 	.sr dd 0.0
	; 	.sg dd 0.0
	; 	.sb dd 0.0
	; 	.font_ptr db 6	
	; window_packet_text_field_cvc:
	; 	.r dd 0.2
	; 	.g dd 0.6
	; 	.b dd 1.0
	; 	.x dd -0.9
	; 	.y dd -0.1
	; 	.tr dd 1.0
	; 	.tg dd 1.0
	; 	.tb dd 1.0
	; 	.length dd 0.1 
	; 	.height dd 0.1
	; 	.sr dd 0.0
	; 	.sg dd 0.0
	; 	.sb dd 0.0
	; 	.font_ptr db 6	
	window_packet_text_field_login:
		.r dd 0.2
		.g dd 0.6
		.b dd 1.0
		.x dd 0.2
		.y dd 0.9
		.tr dd 1.0
		.tg dd 1.0
		.tb dd 1.0
		.length dd 0.5
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6	
	window_packet_text_field_passw:
		.r dd 0.2
		.g dd 0.6
		.b dd 1.0
		.x dd 0.2
		.y dd 0.6
		.tr dd 1.0
		.tg dd 1.0
		.tb dd 1.0
		.length dd 0.5
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6	
window_packet_buttons:
	window_packet_button_register:
		.r dd 0.0
		.g dd 1.0
		.b dd 0.0
		.x dd 0.0
		.y dd -0.5
		.tr dd 0.0
		.tg dd 0.0
		.tb dd 0.0
		.length dd 0.3
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
		.button_r_type db 3
	window_packet_button_exit:
		.r dd 1.0
		.g dd 0.0
		.b dd 0.0
		.x dd 0.9
		.y dd -0.9
		.tr dd 0.0
		.tg dd 0.0
		.tb dd 0.0
		.length dd 0.1
		.height dd 0.1
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
		.button_r_type db 0	
	.window_packet_button_register_text db "Apply", 0
	.window_packet_button_exit_text db "Exit", 0
window_packet_texts:
	window_packet_text_name:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.95
		.font_ptr db 6
	window_packet_text_date:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.65
		.font_ptr db 6
	window_packet_text_phone:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.45
		.font_ptr db 6
	; window_packet_text_card:
	; 	.r dd 0.0
	; 	.g dd 0.0
	; 	.b dd 0.0
	; 	.x dd -0.9
	; 	.y dd 0.15
	; 	.font_ptr db 6
	; window_packet_text_cvc:
	; 	.r dd 0.0
	; 	.g dd 0.0
	; 	.b dd 0.0
	; 	.x dd -0.9
	; 	.y dd -0.05
	; 	.font_ptr db 6
	window_packet_text_login:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd 0.5
		.y dd 0.95
		.font_ptr db 6
	window_packet_text_passwd:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd 0.5
		.y dd 0.65
		.font_ptr db 6
	window_packet_text_greet:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd 0.0
		.y dd 0.95
		.font_ptr db 6
	window_packet_text_error:
		.r dd 1.0
		.g dd 0.0
		.b dd 0.0
		.x dd 0.0
		.y dd 0.0 
		.font_ptr db 3
	.window_packet_text_name_text  db "Your full name:", 0
	.window_packet_text_date_text  db "Date of birth(dd.mm.yy):", 0
	.window_packet_text_phone_text db "Phone number:", 0
	; .window_packet_text_card_text  db "Bank card number:", 0
	; .window_packet_text_cvc_text   db "Bank card CVC:", 0
	.window_packet_text_login_text db "Username:", 0
	.window_packet_text_passw_text db "Password:", 0
	.window_packet_text_greet_text db "Please register", 0
	.window_packet_text_error_text db "Username not unique, or date/phone not valid!", 0
totallen equ $-$$