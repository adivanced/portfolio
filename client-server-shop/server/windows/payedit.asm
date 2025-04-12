sz dq totallen
whindow_packet_header:
	.n_text_fields db 2
	.n_buttons db 3
	.n_text db 3
	.n_boxes db 0
	.r dd 0.0
	.g dd 1.0
	.b dd 1.0
window_packet_text_fields:
	whindow_packet_text_field_card:
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
	whindow_packet_text_field_cvc:
		.r dd 0.2
		.g dd 0.6
		.b dd 1.0
		.x dd -0.3
		.y dd 0.4
		.tr dd 1.0
		.tg dd 1.0
		.tb dd 1.0
		.length dd 0.1
		.height dd 0.2
		.sr dd 0.0
		.dg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
window_packet_buttons:
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
		.button_r_type db 10
	window_packet_button_apply:
		.r dd 0.0
		.g dd 1.0
		.b dd 0.0
		.x dd -0.3
		.y dd 0.0
		.tr dd 0.0
		.tg dd 0.0
		.tb dd 0.0
		.length dd 0.6
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
		.button_r_type db 11
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
	.back_text db "Back", 0
	.apply_text db "Apply changes", 0
	.exit_text db "Exit", 0
window_packet_texts:
	window_packet_text_crd:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.3
		.y dd 0.75
		.font_ptr db 6
	window_packet_text_cvc:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.3
		.y dd 0.45
		.font_ptr db 6
	window_packet_text_greet:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd 0.0
		.y dd 0.9
		.font_ptr db 6
	.crd_text db "Card number:", 0
	.cvc_text db "CVC:", 0
	.greet_text db "Edit/set payment credentials", 0
totallen equ $-$$