.code

fwrap macro a, b
	sub rsp, 10h
	cvtss2sd xmm0, xmm0
	movsd qword ptr [rsp], xmm0
	fld qword ptr [rsp]
	a
	b
	fstp qword ptr [rsp]
	movsd xmm0, qword ptr [rsp]
	cvtsd2ss xmm0, xmm0
	add rsp, 10h
	ret
endm

__sqrtf proc
	fwrap fsqrt
__sqrtf endp

__sinf proc
	fwrap fsin
__sinf endp

__cosf proc
	fwrap fcos
__cosf endp

__tanf proc
	fwrap fptan, fmulp
__tanf endp

arg1 macro
	cvtss2sd xmm1, xmm1
	movsd qword ptr [rsp], xmm1
	fld qword ptr [rsp]
endm

__atan2f proc
	fwrap arg1, fpatan
__atan2f endp

end