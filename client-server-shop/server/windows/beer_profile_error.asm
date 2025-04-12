sz dq totallen
window_packet_header:
	.n_text_fields db 1
	.n_buttons db 3
	.n_text db 3
	.n_boxes db 0
	.r dd 0.0
	.g dd 1.0
	.b dd 1.0
window_packet_text_fields:
	window_packet_text_field_amount:
		.r dd 0.2
		.g dd 0.6
		.b dd 1.0
		.x dd -0.3
		.y dd 0.0
		.tr dd 1.0
		.tg dd 1.0
		.tb dd 1.0
		.length dd 0.6
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
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
	window_packet_button_accept:
		.r dd 0.0
		.g dd 1.0
		.b dd 0.0
		.x dd -0.3
		.y dd -0.3
		.tr dd 0.0
		.tg dd 0.0
		.tb dd 0.0
		.length dd 0.6
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
		.button_r_type db 15
	.exit_text db "Exit", 0
	.back_text db "Back", 0
	.acpt_text db "Add to cart", 0
window_packet_texts:
	whindow_packet_text_accept:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.3
		.y dd 0.05
		.font_ptr db 6
	whindow_packet_text_error:
		.r dd 1.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.3
		.y dd -0.55
		.font_ptr db 3
	whindow_packet_text_beer:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.9
		.font_ptr db 3
	.accept_text db "Enter the amount of beer to order:", 0
	.error_text db "Could not add inserted amount of beer to cart!", 0

totallen equ $-$$



