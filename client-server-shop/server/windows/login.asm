sz dq totallen
whindow_packet_header:
	.n_text_fields db 2
	.n_buttons db 2
	.n_text db 3
	.n_boxes db 0
	.r dd 0.0
	.g dd 1.0
	.b dd 1.0
whindow_packet_text_fields:
	whindow_packet_text_field_unm:
		.r dd 0.2
		.g dd 0.6
		.b dd 1.0
		.x dd -0.3
		.y dd 0.7
		.tr dd 1.0
		.tg dd 1.0
		.tb dd 1.0
		.length dd 0.6
		.height dd 0.2
		.sr dd 0.0
		.dg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
	whindow_packet_text_field_pwd:
		.r dd 0.2
		.g dd 0.6
		.b dd 1.0
		.x dd -0.3
		.y dd 0.3
		.tr dd 1.0
		.tg dd 1.0
		.tb dd 1.0
		.length dd 0.6
		.height dd 0.2
		.sr dd 0.0
		.dg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
whindow_packet_buttons:
	whindow_packet_button_submit:
		.r dd 0.0
		.g dd 1.0
		.b dd 0.0
		.x dd -0.3
		.y dd -0.1
		.tr dd 0.0
		.tg dd 0.0
		.tb dd 0.0
		.length dd 0.6
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
		.button_r_type db 4
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
	.whindow_packet_button_submit_text db "Apply", 0
	.window_packet_button_exit_text db "Exit", 0
window_packet_texts:
	window_packet_text_greet:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd 0.0
		.y dd 0.95
		.font_ptr db 6
	window_packet_text_unm:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.3
		.y dd 0.75
		.font_ptr db 6
	window_packet_text_pwd:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.3
		.y dd 0.35
		.font_ptr db 6
	.window_packet_text_greet_text  db "Log in please", 0
	.window_packet_text_name_text  db "Username:", 0
	.window_packet_text_pwd_text db "Password:", 0
totallen equ $-$$