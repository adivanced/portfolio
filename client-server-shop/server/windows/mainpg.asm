sz dq totallen
whindow_packet_header:
	.n_text_fields db 0
	.n_buttons db 5
	.n_text db 1
	.n_boxes db 0
	.r dd 0.0
	.g dd 1.0
	.b dd 1.0
whindow_packet_text_fields:
whindow_packet_buttons:
	whindow_packet_button_catlogue:
		.r dd 0.0
		.g dd 0.0
		.b dd 1.0
		.x dd -0.5
		.y dd 0.6
		.tr dd 0.0
		.tg dd 0.0
		.tb dd 0.0
		.length dd 0.4 
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
		.button_r_type db 5
	whindow_packet_button_cart:
		.r dd 1.0
		.g dd 0.0
		.b dd 1.0
		.x dd 0.1
		.y dd 0.6
		.tr dd 0.0
		.tg dd 0.0
		.tb dd 0.0
		.length dd 0.4
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
		.button_r_type db 6
	whindow_packet_button_profile:
		.r dd 0.0
		.g dd 1.0
		.b dd 0.0
		.x dd -0.25
		.y dd 0.0
		.tr dd 0.0
		.tg dd 0.0
		.tb dd 0.0
		.length dd 0.4
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
		.button_r_type db 7
	whindow_packet_button_exit:
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
	whindow_packet_button_chat:
		.r dd 0.0
		.g dd 0.0
		.b dd 1.0
		.x dd -1.0
		.y dd -0.9
		.tr dd 0.0
		.tg dd 0.0
		.tb dd 0.0
		.length dd 0.3
		.height dd 0.1
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
		.button_r_type db 21	
	.catalogue_text db "Catalogue", 0
	.cart_text db "Your cart", 0
	.profile_text db "Your profile", 0
	.exit_text db "Exit", 0
	.chat_text db "Support chat", 0
whindow_packet_texts:
	whindow_packet_text_greet:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd 0.0
		.y dd 0.8
		.font_ptr db 6
	.greet_text db "Shop main page", 0
totallen equ $-$$