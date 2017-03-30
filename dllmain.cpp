#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include "MemoryMgr.h"
#include "Maths.h"

double TriangleRadar(CVector2D& pos)
{
	double	dResult = sqrt(pos.x * pos.x + pos.y * pos.y);

	if ( dResult > 1.1547005383792515290182975610039 )
	{
		pos.x /= dResult * (1.0/1.1547005383792515290182975610039);
		pos.y /= dResult * (1.0/1.1547005383792515290182975610039);
	}

	if ( pos.x > 1.0 )
		pos.x = 1.0;
	else if ( -1.0 > pos.x )
		pos.x = -1.0;

	if ( -1.0 > pos.y )
		pos.y = -1.0;

	if ( pos.x > 0.0 )
	{
		if ( pos.y > ((-2.0 * pos.x) + 1.0) )
			pos.y = (-2.0 * pos.x) + 1.0;
	}
	else
	{
		if ( pos.y > ((2.0 * pos.x) + 1.0) )
			pos.y = (2.0 * pos.x) + 1.0;
	}

	return dResult;
}

void __declspec(naked) TriangleRadarMask()
{
	static const float fPositions[] = { 0.0, 1.0, 0.0, 1.0, 1.0, -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0 };
	_asm
	{
		lea		eax, [ebx*8+fPositions-8]
		lea		ecx, [esp+38h]
		push	eax
		push	ecx
		mov		eax, 583480h
		call	eax
		lea		eax, [ebx*8+fPositions+8]
		lea		ecx, [esp+48h]
		push	eax
		push	ecx
		mov		eax, 583480h
		call	eax
		lea		eax, [ebx*8+fPositions+18h]
		lea		ecx, [esp+58h]
		push	eax
		push	ecx
		mov		eax, 583480h
		call	eax
		add		esp, 18h
		push	585888h
		retn
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if ( reason == DLL_PROCESS_ATTACH)
	{
		if (*(uint32_t*)DynBaseAddress(0x82457C) == 0x94BF || *(uint32_t*)DynBaseAddress(0x8245BC) == 0x94BF)
		{
			ScopedUnprotect::Section Protect( GetModuleHandle( nullptr ), ".text" );

			using namespace Memory;

			// Triangle radar
			InjectHook(0x583313, &TriangleRadar, PATCH_JUMP);

			// Texture mask
			InjectHook(0x585850, &TriangleRadarMask, PATCH_JUMP);
			Patch<uint32_t>(0x58581A + 1, 2);
			Patch<uint8_t>(0x585894 + 1, 3); // Num of vertices
			Patch<uint8_t>(0x58589B + 1, 3);
		}
	}
	return TRUE;
}