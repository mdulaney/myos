typedef int64_t greg_t;

struct regs { /* * Extra frame for mdb to follow through high level interrupts and * system traps.  Set them to 0 to terminate stacktrace.  */ 
  greg_t	r_savfp;	/* a copy of %rbp */
  greg_t	r_savpc;	/* a copy of %rip */
  greg_t	r_rdi;		/* 1st arg to function */
  greg_t	r_rsi;		/* 2nd arg to function */
  greg_t	r_rdx;		/* 3rd arg to function, 2nd return register */
	greg_t	r_rcx;		/* 4th arg to function */

	greg_t	r_r8;		/* 5th arg to function */
	greg_t	r_r9;		/* 6th arg to function */
	greg_t	r_rax;		/* 1st return register, # SSE registers */
	greg_t	r_rbx;		/* callee-saved, optional base pointer */

	greg_t	r_rbp;		/* callee-saved, optional frame pointer */
	greg_t	r_r10;		/* temporary register, static chain pointer */
	greg_t	r_r11;		/* temporary register */
	greg_t	r_r12;		/* callee-saved */

	greg_t	r_r13;		/* callee-saved */
	greg_t	r_r14;		/* callee-saved */
	greg_t	r_r15;		/* callee-saved */

	///*
	// * fsbase and gsbase are sampled on every exception in DEBUG kernels
	// * only.  They remain in the non-DEBUG kernel to avoid any flag days.
	// */
	//greg_t	__r_fsbase;	/* no longer used in non-DEBUG builds */
	//greg_t	r_es;
	//greg_t	__r_gsbase;	/* no longer used in non-DEBUG builds */
	//greg_t	r_ds;
	//greg_t	r_fs;		/* %fs is *never* used by the kernel */
	//greg_t	r_gs;

	//greg_t	r_trapno;

	///*
	// * (the rest of these are defined by the hardware)
	// */
	//greg_t	r_err;
	//greg_t	r_rip;
	//greg_t	r_cs;
	//greg_t	r_rfl;
	//greg_t	r_rsp;
	//greg_t	r_ss;
};

