
.code

	; ��win32���ù��������õ�rdi rsp��ʽ
	call_native_func proc 

		push        rdi  
		mov         rdi,rsp  
		
		
		;���ô������Ĳ���Ϊ����
		mov			rax,rcx

		;���������µ���λ�� ��һ��C, �ڶ��� cpps_domain��ǰ��  ������cpps_stack��ջ
		mov			rcx,rdx
		mov			rdx,r8
		mov			r8,r9

	
		call		rax

		pop         rdi  
		ret

	call_native_func endp
	

end

