sz dq totallen
window_packet_header:
	.n_text_fields db 0
	.n_buttons db 2
	.n_text db 1
	.n_boxes db 0
	.r dd 0.0
	.g dd 1.0
	.b dd 1.0
window_packet_text_fields:
window_packet_buttons:
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
	window_packet_button_back:
		.r dd 0.59
		.g dd 0.85
		.b dd 0.48
		.x dd -1.0
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
		.button_r_type db 8
	.exit_text db "Exit", 0
	.back_text db "Back", 0
window_packet_texts:
	whindow_packet_text_greet:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd 0.0
		.y dd 0.95
		.font_ptr db 6
totallen equ $-$$
