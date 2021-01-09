
.code

	; 从win32调用过来，采用的rdi rsp方式
	call_native_func proc 

		push        rdi  
		mov         rdi,rsp  
		
		
		;设置传过来的参数为函数
		mov			rax,rcx

		;将参数重新调整位置 第一个C, 第二个 cpps_domain当前域  第三个cpps_stack堆栈
		mov			rcx,rdx
		mov			rdx,r8
		mov			r8,r9

	
		call		rax

		pop         rdi  
		ret

	call_native_func endp
	

end

