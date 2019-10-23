#ifndef CPPS_JIT_COMPILER_CPPS_HEAD_
#define CPPS_JIT_COMPILER_CPPS_HEAD_

//===================================
//@Author		:	Johnson
//@QQ			:	88481106
//@Email		:	jiang_4177@163.com
//@Date			:	2016/1/6 (yy/mm/dd)
//@Module		:	CPPS_JIT_COMPILER
//@Description	:	Cpps函数Just in Time Compiler(JIT 即时编译） 暂时仅支持x64。
//@website		:	http://cpps.wiki
//==================================


namespace cpps
{
	struct C;
	struct cpps_domain;
	struct Node;

	struct cpps_jit_compiler
	{
		cpps_jit_compiler()
		:stack_offset(0x40) //默认48字节给系统用 包括调用函数的参数
		{
			//填充参数
			mov_rsp_off_rcx(0x30);
			mov_rsp_off_rdx(0x38);
			mov_rsp_off_r8(0x40);
		}

		void			compiler(C *c, cpps_domain *domain, Node *p);
		void			compiler_dofunction(C * c, cpps_domain * domain, Node * o);
		void			compiler_params(C * c, cpps_domain * domain, Node * v);
		void			compiler_vector_pushback(C * c, cpps_domain * domain, Node * v, usint32 vector_offset);



		void			push(usint8 opCode)
		{
			push(opCode, asmVec);
		}
		void			push(usint8 opCode, std::vector<usint8> &vec)
		{
			vec.push_back(opCode);
		}
		void			push_int(usint32 i)
		{
			push_int(i, asmVec);
		}
		void			push_int(usint32 i, std::vector<usint8> &vec)
		{
			push(((int8 *)&i)[0], vec);
			push(((int8 *)&i)[1], vec);
			push(((int8 *)&i)[2], vec);
			push(((int8 *)&i)[3], vec);
		}
		void			push_int64(usint64 ll)
		{
			push_int64(ll, asmVec);
		}
		void			push_int64(usint64 ll, std::vector<usint8> &vec)
		{
			push(((int8 *)&ll)[0], vec);
			push(((int8 *)&ll)[1], vec);
			push(((int8 *)&ll)[2], vec);
			push(((int8 *)&ll)[3], vec);
			push(((int8 *)&ll)[4], vec);
			push(((int8 *)&ll)[5], vec);
			push(((int8 *)&ll)[6], vec);
			push(((int8 *)&ll)[7], vec);
		}

		void			push_rdi()
		{
			push(0x40);
			push(0x57);
		}
		void			mov_rdi_rsp()
		{
			push(0x48);
			push(0x8B);
			push(0xFC);
		}
		void			pop_rdi()
		{
			push(0x5F);
		}
		void			ret()
		{
			push(0xC3);
		}
		void			call_func(usint64 func)
		{
			mov_rax_ll(func);
			call_rax();
		}
		void			mov_rax_ll(usint64 ll)
		{
			push(0x48);
			push(0xB8);
			push_int64(ll);
		}
		void			mov_rcx_ll(usint64 ll)
		{
			push(0x48);
			push(0xB9);
			push_int64(ll);
		}
		void			mov_rdx_ll(usint64 ll)
		{
			push(0x48);
			push(0xBA);
			push_int64(ll);
		}
		void			mov_r8_ll(usint64 ll)
		{
			push(0x49);
			push(0xB8);
			push_int64(ll);
		}
		void			mov_r9_ll(usint64 ll)
		{
			push(0x49);
			push(0xB9);
			push_int64(ll);
		}
		void			mov_r10_ll(usint64 ll)
		{
			push(0x49);
			push(0xBA);
			push_int64(ll);
		}
		void call_rax()
		{
			push(0xff);
			push(0xd0);
		}
		void lea_rcx_rsp_off(usint32 offset)
		{
			push(0x48);
			push(0x8D);
			push(0x8C);
			push(0x24);

			push_int(offset);
		}
		void mov_rcx_rsp_off(usint32 offset)
		{
			push(0x48);
			push(0x8B);
			push(0x8C);
			push(0x24);

			push_int(offset);
		}
		void mov_rax_rsp_off(usint32 offset)
		{
			push(0x48);
			push(0x8B);
			push(0x84);
			push(0x24);

			push_int(offset);
		}
		void mov_rdx_rsp_off(usint32 offset)
		{
			push(0x48);
			push(0x8B);
			push(0x94);
			push(0x24);

			push_int(offset);
		}

		void mov_r8_rsp_off(usint32 offset)
		{
			push(0x4C);
			push(0x8B);
			push(0x84);
			push(0x24);

			push_int(offset);
		}
		void mov_r9_rsp_off(usint32 offset)
		{
			push(0x4C);
			push(0x8B);
			push(0x8C);
			push(0x24);

			push_int(offset);
		}
		void mov_r10_rsp_off(usint32 offset)
		{
			push(0x4C);
			push(0x8B);
			push(0x94);
			push(0x24);

			push_int(offset);
		}

		//mov [rsp+off],rax
		void mov_rsp_off_rax(usint32 offset)
		{
			push(0x48);
			push(0x89);
			push(0x84);
			push(0x24);

			push_int(offset);
		}
		void mov_rsp_off_rcx(usint32 offset)
		{
			push(0x48);
			push(0x89);
			push(0x8C);
			push(0x24);

			push_int(offset);
		}
		void mov_rsp_off_rdx(usint32 offset)
		{
			push(0x48);
			push(0x89);
			push(0x94);
			push(0x24);

			push_int(offset);
		}
		void mov_rsp_off_r8(usint32 offset)
		{
			push(0x4C);
			push(0x89);
			push(0x84);
			push(0x24);

			push_int(offset);
		}
		void mov_rdx_rax()
		{
			push(0x48);
			push(0x8B);
			push(0xD0);
		}
		void mov_rcx_rax()
		{
			push(0x48);
			push(0x8B);
			push(0xC8);
		}
			
		usint32 reg_stack_size(usint32 size)
		{
			stack_offset += size;
			return stack_offset;
		}
		void*			opcode(std::vector<usint8> &vec)
		{
			return &vec[0];
		}

		void begin(std::vector<usint8> &vec)
		{
			push(0x57, vec);

			push(0x48, vec);
			push(0x81, vec);
			push(0xEC, vec);
			push_int(stack_offset, vec);


			push(0x48, vec);
			push(0x8B, vec);
			push(0xFC, vec);

			
		}

		void end(std::vector<usint8> &vec)
		{
			push(0x81, vec);
			push(0xC4, vec);

			push_int(stack_offset, vec);


			push(0x5F, vec);
			push(0xc3, vec);
		}

		void build(char *&buff, size_t &out_size)
		{
			std::vector<usint8>	buildVec;
			begin(buildVec);

			buildVec.insert(buildVec.end(), asmVec.begin(), asmVec.end());

			end(buildVec);
			
			
			//编译出机器码
#ifdef _WIN32
			buff = (char*)VirtualAlloc(0, buildVec.size(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#else
			buff = (char*)mmap(0, buildVec.size(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif

			memcpy(buff, opcode(buildVec), buildVec.size());
			out_size = buildVec.size();

#ifdef WIN32
			DWORD dwOld;
			VirtualProtect(buff, buildVec.size(), PAGE_EXECUTE_READ, &dwOld);
#else  
			mprotect(buff, buildVec.size(), PROT_READ | PROT_EXEC);
#endif

		}
		void movsd_rdx_xmm0(cpps_number* pNumber)
		{
			push(0xF2);
			push(0xF2);
		}


		std::vector<const char *>		stringVec;//记录一些字符串的。
		usint32				stack_offset;
		std::vector<usint8>	asmVec;
	};
}

#endif // CPPS_JIT_COMPILER_CPPS_HEAD_