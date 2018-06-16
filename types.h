#ifndef _TYPES_H_
#define _TYPES_H_

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef char s8;
typedef short s16;
typedef long s32;
typedef u8 (*IQ_CALLBACK)(u32, u32);

class T_CIRCLE
{
public:
	void	*mapPhys;
	s32		*mapPos;
	u8		*mapOnOff;
	s32		radiusPerso;
	s32		radiusLines;
};

class T_LINE
{
public:
	u16		sX,sY,eX,eY, angle,groupe;
};

class T_LINE_HQ
{
public:
	s32		sX,sY,eX,eY,mX,mY,groupe;
	u8		angle;
	inline T_LINE_HQ&	operator =(const T_LINE_HQ& p)
	{
		sX		= p.sX;
		sY		= p.sY;
		eX		= p.eX;
		eY		= p.eY;
		mX		= p.mX;
		mY		= p.mY;
		groupe	= p.groupe;
		angle	= p.angle;
		return *this;
	};
};

typedef struct tagT_HIT{
	T_CIRCLE		*sphHitten1;
	T_CIRCLE		*sphHitten2;
	T_LINE			*lineHitten;
	s32				hitX, hitY; 
	s32				u1,	u2;
	s16				norX, norY;
	u8				angle;
	u8				valid;
	s16				timeRemaining;
}T_HIT;


#endif // _TYPES_H_
