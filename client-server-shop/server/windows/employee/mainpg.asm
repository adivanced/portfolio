sz dq totallen
whindow_packet_header:
	.n_text_fields db 0
	.n_buttons db 3
	.n_text db 0
	.n_boxes db 0
	.r dd 0.0
	.g dd 1.0
	.b dd 1.0
whindow_packet_text_fields:
whindow_packet_buttons:
	whindow_packet_button_clients:
		.r dd 0.2
		.g dd 0.5
		.b dd 1.0
		.x dd -0.2
		.y dd 0.4
		.tr dd 0.0
		.tg dd 0.0
		.tb dd 0.0
		.length dd 0.4
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
		.button_r_type db 2
	whindow_packet_button_orders:
		.r dd 0.5
		.g dd 1.0
		.b dd 0.2
		.x dd -0.2
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
		.button_r_type db 3
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
	.whindow_packet_button_submit_text db "Clients", 0
	.orders_text db "Orders", 0
	.window_packet_button_exit_text db "Exit", 0
totallen equ $-$$