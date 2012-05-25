#ifdef _MSC_VER 
	//for VC++
	#pragma once
#endif
#ifndef ____OPLEN____72
#define ____OPLEN____72
//
//オペランド長を求めます。
//このプロダクトは sexyhookのサブプロダクトです。
//
//
// by rti
//
//
// new BSD ライセンス / NYSL ライセンス 好きに使えばいいんぢゃない？ ^q^
//
#include <windows.h>
#include <stdio.h>

#define OPLEN_ASSERT(f) \
	if (!(f) ) \
	{ \
		DebugBreak(); \
	} 


//関数/メソッドをフックする.
class SEXYHOOKFuncBase
{
public:
	enum SEXYHOOK_CPU_ARCHITECTURE
	{
		 SEXYHOOK_CPU_ARCHITECTURE_X86
		,SEXYHOOK_CPU_ARCHITECTURE_X64
	};

	//マシン語の長さを求めます。
	//http://www2.odn.ne.jp/~hab81760/modrm_sib.htm
	//http://dl.dropbox.com/u/2476414/TechResources/x86_opcodemap_1_b4.pdf
	//http://download.intel.com/jp/developer/jpdoc/IA32_Arh_Dev_Man_Vol2B_i.pdf
	//http://www.logix.cz/michal/doc/i386/app-a.htm
	//http://www.wdic.org/w/SCI/REX%E3%83%97%E3%83%AA%E3%83%95%E3%82%A3%E3%83%83%E3%82%AF%E3%82%B9
	//http://www.wdic.org/w/SCI/%E3%82%AA%E3%83%9A%E3%82%B3%E3%83%BC%E3%83%89%20(IA-32)
	//http://iwaohanma.web.fc2.com/
	static int OperandLength(const unsigned char* code,SEXYHOOK_CPU_ARCHITECTURE cputype)
	{
		//1バイト命令
		//0x00  仕様がよくわからない命令
		//0x01  1バイトの命令
		//0x02  2バイトの命令
		//...
		//0xff	modrw
		//0xfd	modrw + 4
		//0xfc	modrw + 1
		//0xfb  modrw + 4 or modrw + 1 or 2
		//0xfa  modrw + 1 or 2
		//0xf5	66 の影響を受ける 5バイト長の命令
		//0xe5	66 の影響を受ける 5バイト長の命令  64bitで48をつけると 64bitになる命令
		static unsigned char codeTable[] = {
//		0		1		2		3		4		5		6		7		8		9		a		b		c		d		e		f
/*0*/	0xff,	0xff,	0xff,	0xff,	0x02,	0xf5,	0x01,	0x01,	0xff,	0xff,	0xff,	0xff,	0x02,	0xf5,	0x01,	0x00,//0
/*1*/	0xff,	0xff,	0xff,	0xff,	0x02,	0xf5,	0x01,	0x01,	0xff,	0xff,	0xff,	0xff,	0x02,	0xf5,	0x01,	0x01,//1
/*2*/	0xff,	0xff,	0xff,	0xff,	0x02,	0xf5,	0x01,	0x01,	0xff,	0xff,	0xff,	0xff,	0x02,	0xf5,	0x01,	0x01,//2
/*3*/	0xff,	0xff,	0xff,	0xff,	0x02,	0xf5,	0x01,	0x01,	0xff,	0xff,	0xff,	0xff,	0x02,	0xf5,	0x01,	0x01,//3
/*4*/	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,//4
/*5*/	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,//5
/*6*/	0x01,	0x01,	0xff,	0xff,	0x00,	0x00,	0x00,	0x00,	0x05,	0xfd,	0x02,	0xfc,	0x01,	0x01,	0x00,	0x00,//6
/*7*/	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,//7
/*8*/	0xfc,	0xfd,	0xfd,	0xfc,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,//8
/*9*/	0x01,	0x01,	0x05,	0x05,	0x01,	0x05,	0x05,	0x05,	0x01,	0x01,	0x00,	0x01,	0x01,	0x01,	0x01,	0x01,//9
/*a*/	0x05,	0x05,	0x05,	0x05,	0x01,	0x01,	0x01,	0x01,	0x02,	0xf5,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,//a
/*b*/	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0xe5,	0xe5,	0xe5,	0xe5,	0xe5,	0xe5,	0xe5,	0xe5,//b
/*c*/	0xfc,	0xfc,	0x03,	0x01,	0xff,	0xff,	0xfc,	0xfd,	0x01,	0x01,	0x01,	0x03,	0x01,	0x02,	0x01,	0x01,//c
/*d*/	0xff,	0xff,	0xff,	0xff,	0x02,	0x02,	0x01,	0x01,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,//d
/*e*/	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x02,	0x05,	0x05,	0xf5,	0x02,	0xff,	0xff,	0xff,	0xff,//e
/*f*/	0x01,	0x00,	0x01,	0x00,	0x01,	0x00,	0xfa,	0xfb,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0xff,	0xff //f
		};

		//f0から始まる 2バイト命令
		static unsigned char codeTableF0[] = {
//		0		1		2		3		4		5		6		7		8		9		a		b		c		d		e		f
/*0*/	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x01,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,//0
/*1*/	0xff,	0xff,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0xff,	0x00,	0xff,//1
/*2*/	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0xff,	0xff,	0xff,	0x00,	0xff,	0xff,	0xff,	0xff,//2
/*3*/	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,//3
/*4*/	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,//4
/*5*/	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,//5
/*6*/	0xff,	0xff,	0xff,	0xff,	0x00,	0x00,	0x00,	0xff,	0xff,	0x00,	0x00,	0x00,	0x00,	0x00,	0xff,	0xff,//6
/*7*/	0xfc,	0xfc,	0xfc,	0xfc,	0xff,	0xff,	0xff,	0x01,	0xff,	0xff,	0x00,	0x00,	0x00,	0x00,	0xff,	0xff,//7
/*8*/	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,	0x05,//8
/*9*/	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0x02,	0x02,	0x02,	0x02,	0xff,	0xff,	0xff,	0xff,//9
/*a*/	0x00,	0x00,	0x01,	0xff,	0xfc,	0xff,	0x00,	0x00,	0x00,	0x00,	0x00,	0xff,	0xfc,	0xff,	0xff,	0xff,//a
/*b*/	0x02,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0x00,	0x00,	0xfc,	0xff,	0xff,	0xff,	0xff,	0xff,//b
/*c*/	0x00,	0xff,	0xfc,	0x00,	0x00,	0x00,	0x00,	0xff,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,	0x01,//c
/*d*/	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,//d
/*e*/	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,//e
/*f*/	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0xff,	0x00,	0x00 //f
		};

		unsigned char len = codeTable[*code];
		unsigned char ex = 0;
		unsigned char registersize = 0;
		unsigned char registersize64 = 0;

		if (cputype == SEXYHOOK_CPU_ARCHITECTURE_X64)
		{//64bit対応 その1
			if (*code == 0xf2 || *code == 0xf3)
			{
				code =	code + 1;
				ex += 1;

				len = codeTable[*code];
			}
		}

		//prefix ぷよぷよだとダイアキュートみたいな
		if (*code >= 0x64 && *code <= 0x67 )
		{
			registersize = *code;

			code =	code + 1;
			ex += 1;
			len = codeTable[*code];
		}

		if (cputype == SEXYHOOK_CPU_ARCHITECTURE_X64)
		{//64bit対応 その2
			if ( registersize == 0x66 && *code == 0x2e)
			{//66 2e 0f 1f 84 00 00 00 00 00  長いnot のように、 66 2e と入るときがあるらしい。
				code =	code + 1;
				ex += 1;
			}

			if (*code >= 0x40 && *code <= 0x4f )
			{
				registersize64 = *code;

				code =	code + 1;
				ex += 1;
				len = codeTable[*code];
			}
		}

		//lock prefix
		if (*code == 0xf0)
		{
			code = code + 1;
			ex += 1;
			len = codeTable[*code];
		}

		//9b は謎w 
		if (*code == 0x9b)
		{
			if (*(code+1) == 0xdd)
			{// 9b dd b4 24 50 02 00 00  fsave  0x250(%rsp)
				return ex + 2 + 2 + 4;
			}
			else if (*(code+1) == 0x97)
			{//9b 97 66 00 ???
				return ex + 1 + 3;
			}
			else if (*(code+1) == 0x3f)
			{//9b 3f 08 ???
				return ex + 1 + 2;
			}
			else if (*(code+1) == 0xc4)
			{//9b c4 20   ???
				return ex + 1 + 2;
			}
			else
			{
				return ex + 1;
			}
		}

		//2バイトの長いオペコードの処理
		if (*code == 0x0f)
		{
			code = code + 1;
			ex += 1;
			len = codeTableF0[*code];
		}

		//repne / reps は 次のコードを見ないとわからない
		if (*code == 0xf2 || *code == 0xf3)
		{
			if ( (*(code + 1) & 0xf0)  == 0xa0)
			{
				return ex + 2;
			}
			return ex + 1;
		}

		if (len == 0)
		{
		}
		else if ( len <= 0x10)
		{
			return len + ex;
		}
		else if ( len == 0xf5 || len == 0xe5)
		{
			if (registersize == 0x66)
			{//66 bf と来ると word 単位でアクセスするらしい
				return 3 + ex;
			}
			else
			{
				if (len == 0xe5 && (registersize64 >= 0x48 && registersize64 <= 0x4f) )
				{//64bit拡張で 8バイトでアクセスできる命令
					return 9 + ex;
				}
				return 5 + ex;
			}
		}
		else
		{
			unsigned char mod = 0;
			unsigned char reg = 0;
			unsigned char rw =  0;
			int modlen = modrwLen( *(code + 1) ,*(code + 2) ,&mod,&reg,&rw);

			if ( len == 0xff)
			{
				return 1 + 1 + modlen + ex;
			}
			if ( len == 0xfd)
			{//c7 85 00 fc ff ff 00 00 00 00
			 //mov         dword ptr [ebp-400h],0
				if (registersize == 0x66)
				{//66 c7 と来ると word 単位でアクセスするらしい
					return 1 + 1 + modlen + 2 + ex;
				}
				else
				{
					return 1 + 1 + modlen + 4 + ex;
				}
			}
			if ( len == 0xfc)
			{//c1 f8 0a
			 //sar	eax,0x0a
				return 1 + 1 + modlen + 1 + ex;
			}
			if ( len == 0xfb)
			{//modrw によって長さが変わる
				if (reg == 0)
				{
					if (registersize == 0x66)
					{
						return 1 + 1 + 2 + modlen + ex;
					}
					else
					{
						return 1 + 1 + 4 + modlen + ex;
					}
				}
				else if (reg >= 2) return 1 + 1 + modlen + ex;
				else return 1 + 1 + ex;
			}
			if ( len == 0xfa)
			{//modrw によって長さが変わる
				if (reg == 0) return 1 + 1 + 1 + modlen + ex;
				else if (reg >= 2) return 1 + 1 + modlen + ex;
				else return 1 + 1 + ex;
			}
		}

		DebugBreak();
		return 0;
	}

	static int modrwLen(unsigned char modrw,unsigned char sib,unsigned char *mod,unsigned char *reg,unsigned char *rw) 
	{
		*mod = (modrw & 0xc0) >> 6;
		*reg = (modrw & 0x38) >> 3;
		*rw =  (modrw & 0x07);

		switch (*mod)
		{
		case 0:
			switch(*rw)
			{
			case 0:		return 0;//eax
			case 1:		return 0;//ecx
			case 2:		return 0;//edx
			case 3:		return 0;//ebx
			case 4:
				{//SIB    sibのbaseビットによって長さが違う
					unsigned char base = (sib & 0x07);
					if (base == 5)	return 1 + 4;
					else			return 1;
				}
			case 5:		return 4;//disp32
			case 6:		return 0;//esi
			case 7:		return 0;//edi
			}
			break;
		case 1:
			switch(*rw)
			{
			case 0:		return 1;//eax + disp8
			case 1:		return 1;//ecx + disp8
			case 2:		return 1;//edx + disp8
			case 3:		return 1;//ebx + disp8
			case 4:		return 1 + 1;//SIB
			case 5:		return 1;//ebp + disp8
			case 6:		return 1;//esi + disp8
			case 7:		return 1;//edi + disp8
			}
		case 2:
			switch(*rw)
			{
			case 0:		return 4;//eax + disp32
			case 1:		return 4;//ecx + disp32
			case 2:		return 4;//edx + disp32
			case 3:		return 4;//ebx + disp32
			case 4:		return 1 + 4;//SIB
			case 5:		return 4;//ebp + disp32
			case 6:		return 4;//esi + disp32
			case 7:		return 4;//edi + disp32
			}
		case 3:
			switch(*rw)
			{
			case 0:		return 0;//al/ax/eax/mm0/xmm0
			case 1:		return 0;//cl/cx/ecx/mm1/xmm1
			case 2:		return 0;//dl/dx/edx/mm2/xmm2
			case 3:		return 0;//bl/bx/ebx/mm3/xmm3
			case 4:		return 0;//ah/sp/esp/mm4/xmm4
			case 5:		return 0;//ch/bp/ebp/mm5/xmm5
			case 6:		return 0;//dh/si/esi/mm6/xmm6
			case 7:		return 0;//bh/di/edi/mm7/xmm7
			}
		}
		DebugBreak();
		return 0;
	}
};

#endif //____OPLEN____72
