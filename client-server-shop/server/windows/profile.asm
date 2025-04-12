sz dq totallen
whindow_packet_header:
	.n_text_fields db 0
	.n_buttons db 4
	.n_text db 15
	.n_boxes db 0
	.r dd 0.0
	.g dd 1.0
	.b dd 1.0
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
		.button_r_type db 8
	window_packet_button_edit:
		.r dd 0.0
		.g dd 0.0
		.b dd 1.0
		.x dd -0.2
		.y dd -0.5
		.tr dd 0.0
		.tg dd 0.0
		.tb dd 0.0
		.length dd 0.4
		.height dd 0.2
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
		.button_r_type db 9			
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
	window_packet_button_delete:
		.r dd 1.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.8
		.y dd -0.1
		.tr dd 0.0
		.tg dd 0.0
		.tb dd 0.0
		.length dd 0.31
		.height dd 0.1
		.sr dd 0.0
		.sg dd 0.0
		.sb dd 0.0
		.font_ptr db 6
		.button_r_type db 20
	.back_text db "Back", 0
	.edit_text db "Edit/set payment data", 0
	.exit_text db "Exit", 0
	.delt_text db "DELETE PROFILE", 0
window_packet_texts:
	whindow_packet_text_greet:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd 0.0
		.y dd 0.9
		.font_ptr db 6
	window_packet_text_unm:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.8
		.font_ptr db 6
	window_packet_text_pwd:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.7
		.font_ptr db 6
	window_packet_text_nm:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.6
		.font_ptr db 6
	window_packet_text_dt:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.5
		.font_ptr db 6
	window_packet_text_ph:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.4
		.font_ptr db 6
	window_packet_text_crd:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.3
		.font_ptr db 6
	window_packet_text_cvc:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.2
		.font_ptr db 6
	window_packet_text_unm_d:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.75
		.font_ptr db 6
	window_packet_text_pwd_d:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.65
		.font_ptr db 6
	window_packet_text_nm_d:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.55
		.font_ptr db 6
	window_packet_text_dt_d:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.45
		.font_ptr db 6
	window_packet_text_phn_d:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.35
		.font_ptr db 6
	window_packet_text_crd_d:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.25
		.font_ptr db 6
	window_packet_text_cvc_d:
		.r dd 0.0
		.g dd 0.0
		.b dd 0.0
		.x dd -0.9
		.y dd 0.15
		.font_ptr db 6
	.greet_text db "Your profile", 0
	.unm_text db "Username:", 0
	.pwd_text db "Password", 0
	.nm_text db "Name:", 0
	.dt_text db "Date of birth:", 0
	.ph_text db "Phone:", 0
	.crd_text db "Card:", 0
	.cvc_text db "CVC:", 0
totallen equ $-$$
