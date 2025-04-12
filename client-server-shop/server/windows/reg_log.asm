sz dq totallen
whindow_packet_header:
	.n_text_fields db 0
	.n_buttons db 3
	.n_text db 1
	.n_boxes db 0
	.r dd 0.0
	.g dd 1.0
	.b dd 1.0
window_packet_text_fields:
window_packet_buttons:
	window_packet_button_login:
		.r dd 0.0
		.g dd 1.0
		.b dd 0.0
		.x dd -0.6
		.y dd 0.1
		.tr dd 0.0
		.tg dd 0.0
		.tb dd 0.0
		.length dd 0.3
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
		.button_r_type db 2
		;.button_n_max_r dq 0

	window_packet_button_register:
		.r dd 0.0
		.g dd 0.0
		.b dd 1.0
		.x dd 0.1
		.y dd 0.1
		.tr dd 0.0
		.tg dd 0.0
		.tb dd 0.0
		.length dd 0.3
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
		.button_r_type db 1
		;.button_n_max_r dq 0

	window_packet_button_exit:
		.r dd 1.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.25
		.y dd -0.6
		.tr dd 0.0
		.tg dd 0.0
		.tb dd 0.0
		.length dd 0.5
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
		.button_r_type db 0
		;.button_n_max_r dq 0
	.window_packet_button_login_text db "Log in", 0
	.window_packet_button_register_text db "Register", 0
	.window_packet_button_exit_text db "Exit", 0



	window_packet_text:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.1
		.y dd 0.6
		.font_ptr db 6
	.window_packet_text_text db "Log in, register or exit.", 0

totallen equ $-$$