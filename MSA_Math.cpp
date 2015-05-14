__declspec(naked) int __fastcall _Floor(float &f)				// int i=(int)f; if(i>f) i--;
{
	__asm {
		fld			dword ptr[ecx]
		frndint
		fcom		dword ptr[ecx]
		fistp		dword ptr[esp-4]
		fnstsw      ax
		test        ah,41h
		jne         ItsOk
		dec			dword ptr[esp-4]
	ItsOk:
		mov			eax,dword ptr[esp-4]
		ret	
	}
}

__declspec(naked) int __fastcall _Ceil(float &f)				// int i=(int)f; if(i>f) i--;
{
	__asm {
		fld			dword ptr[ecx]
		frndint
		fcom		dword ptr[ecx]
		fistp		dword ptr[esp-4]
		fnstsw      ax
		test        ah,1
		je         ItsOk
		inc			dword ptr[esp-4]
	ItsOk:
		mov			eax,dword ptr[esp-4]
		ret
	}
}
