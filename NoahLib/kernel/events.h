#pragma once

#define DescRC   	0xf2000000
#define RCntCNT2  	(DescRC|0x02)  	/* one-eighth of system clock */

#define EvSpINT		0x0002		/* interrupted */

#define RCntMdINTR	0x1000

typedef void(*eventFunc)();

u32 OpenEvent(u32 desc, u32 spec, u32 mode, eventFunc func = nullptr);
u32 CloseEvent(u32 event);
u32 EnableEvent(u32 event);

u32 SetRCnt(u32 spec, u16 target, u32 mode);
u32 StartRCnt(u32 spec);