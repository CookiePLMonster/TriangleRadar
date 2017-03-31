#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include "MemoryMgr.h"
#include "Maths.h"

static void (*const TransformRadarPointToScreenSpace)(CVector2D&,const CVector2D&) = (void(*)(CVector2D&,const CVector2D&))0x583480;

static float sign (const CVector2D& p1, const CVector2D& p2, const CVector2D& p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

static float clamp( float v, float lo, float hi )
{
	return v < lo ? lo : hi < v ? hi : v;
}

double TriangleRadar(CVector2D& pos)
{
	const double	dResult = sqrt(pos.x * pos.x + pos.y * pos.y);

	constexpr CVector2D pointA( 0.0, 1.0 );
	constexpr CVector2D pointB( 1.0, -1.0 );
	constexpr CVector2D pointC( -1.0, -1.0 );

	const bool bA = sign(pos, pointA, pointB) < 0.0f;
	const bool bB = sign(pos, pointB, pointC) < 0.0f;
	const bool bC = sign(pos, pointC, pointA) < 0.0f;

	if ( bA && bB && bC )
	{
		// Point is inside the triangle, don't calculate anything
		return dResult;
	}

	if ( !bA )
	{
		constexpr float m = (pointB.y - pointA.y) / (pointB.x - pointA.x);
		constexpr float b = pointA.y - (m * pointA.x);

		const float newX = (m * pos.y + pos.x - m * b) / (m * m + 1);
		const float newY = (m * m * pos.y + m * pos.x + b) / (m * m + 1);

		pos.x = clamp( newX, 0.0f, 1.0f );
		pos.y = clamp( newY, -1.0f, 1.0f );
	}
	else if ( !bC )
	{
		constexpr float m = (pointA.y - pointC.y) / (pointA.x - pointC.x);
		constexpr float b = pointC.y - (m * pointC.x);

		const float newX = (m * pos.y + pos.x - m * b) / (m * m + 1);
		const float newY = (m * m * pos.y + m * pos.x + b) / (m * m + 1);	

		pos.x = clamp( newX, -1.0f, 0.0f );
		pos.y = clamp( newY, -1.0f, 1.0f );
	}
	else if ( !bB )
	{
		constexpr float m = (pointC.y - pointB.y) / (pointC.x - pointB.x);
		constexpr float b = pointB.y - (m * pointB.x);

		const float newX = (m * pos.y + pos.x - m * b) / (m * m + 1);

		pos.x = clamp( newX, -1.0f, 1.0f );
		pos.y = -1.0f;
	}

	return dResult;
}

static void* const TriangleRadarMask_JumpBack = (void*)0x585888;
void __declspec(naked) TriangleRadarMask()
{
	static const float fPositions[] = { 0.0, 1.0, 0.0, 1.0, 1.0, -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0 };
	_asm
	{
		lea		eax, [ebx*8+fPositions-8]
		lea		ecx, [esp+38h]
		push	eax
		push	ecx
		call	TransformRadarPointToScreenSpace

		lea		eax, [ebx*8+fPositions+8]
		lea		ecx, [esp+48h]
		push	eax
		push	ecx
		call	TransformRadarPointToScreenSpace

		lea		eax, [ebx*8+fPositions+18h]
		lea		ecx, [esp+58h]
		push	eax
		push	ecx
		call	TransformRadarPointToScreenSpace

		add		esp, 18h
		jmp		TriangleRadarMask_JumpBack
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

			// No radardisc
			InjectHook(0x58A782, 0x58AA2A, PATCH_JUMP );
		}
	}
	return TRUE;
}