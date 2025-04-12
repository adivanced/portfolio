sz dq totallen
whindow_packet_header:
	.n_text_fields dq 2
	.n_buttons dq 2
	.n_text dq 3
	.n_boxes dq 0
window_packet_text_fields:
	window_packet_text_field_name:
		.r dd 0.2
		.g dd 0.6
		.b dd 1.0
		.x dd -0.4
		.y dd 0.4
		.tr dd 1.0
		.tg dd 1.0
		.tb dd 1.0
		.length dd 0.8
		.height dd 0.2 
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr dq 6
	window_packet_text_field_password:
		.r dd 0.2
		.g dd 0.6
		.b dd 1.0
		.x dd -0.4
		.y dd 0.1
		.tr dd 1.0
		.tg dd 1.0
		.tb dd 1.0
		.length dd 0.8
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr dq 6
window_packet_buttons:
	window_packet_button_register:
		.r dd 0.0
		.g dd 1.0
		.b dd 0.0
		.x dd -0.2
		.y dd -0.2
		.tr dd 0.0
		.tg dd 0.0
		.tb dd 0.0
		.length dd 0.3
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr dq 6
		.button_r_type dq 3
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
		.font_ptr dq 6
		.button_r_type dq 0
	.window_packet_button_register_text db "Apply", 0
	.window_packet_button_exit_text db "Exit", 0
window_packet_text:
	window_packet_text_name:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.1
		.y dd 0.5
		.font_ptr dq 6
	window_packet_text_pwd:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.1
		.y dd 0.15
		.font_ptr dq 6
	window_packet_text_greet:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd 0.0
		.y dd 0.9
		.font_ptr dq 6
	.window_packet_text_name_text db "Username:", 0
	.window_packet_text_pwd_text db "Password:", 0
	.window_packet_text_greet_text db "Please Register", 0

totallen equ $-$$