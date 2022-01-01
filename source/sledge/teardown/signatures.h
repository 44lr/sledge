#pragma once

#include "util/memory.h"

namespace Signatures {
	/*
		memory sigs
	*/
	inline CSignature free("\xE8\x00\x00\x00\x00\x4C\x39\x26", "x????xxx");
	inline CSignature alloc("\xE8\x00\x00\x00\x00\xEB\x1F\x48\x85\xDB", "x????xxxxx");
	inline CSignature calloc("\xE8\x00\x00\x00\x00\x33\xC9\x48\x8B\xE8", "x????xxxxx");
	inline CSignature realloc("\xE8\x00\x00\x00\x00\x48\x8B\xF0\x48\x85\xC0\x74\x16", "x????xxxxxxxx");
	inline CSignature memmove("\x4C\x8B\xD9\x4C\x8B\xD2", "xxxxxx", false);

	/*
		sigs used for callbacks
	*/
	inline CSignature Update("\xE8\x00\x00\x00\x00\x48\x8D\x4D\xA7\xE8\x00\x00\x00\x00\xEB\x1E", "x????xxxxx????xx");
	inline CSignature SpawnPlayer("\xE8\x00\x00\x00\x00\xC6\x86\x00\x00\x00\x00\x00\x48\x8B\xCE", "x????xx?????xxx");
	inline CSignature UpdatePlayer("\xE8\x00\x00\x00\x00\x48\x8B\x4B\x78\xF3\x0F\x10\x8B\x00\x00\x00\x00", "x????xxxxxxxx????");
	
	/*
		class constructors
	*/
	inline CSignature Game("\xE8\x00\x00\x00\x00\x48\x8B\xF8\xEB\x02\x33\xFF\x48\x8D\x4D\xA7", "x????xxxxxxxxxxx");

	inline CSignature Body("\xE8\x00\x00\x00\x00\x48\x8B\xF8\x33\xDB", "x????xxxxx");
	inline CSignature Shape("\xE8\x00\x00\x00\x00\x48\x8B\xF8\x48\x89\x7D\xA0", "x????xxxxxxx");
	inline CSignature Light("\xE8\x00\x00\x00\x00\xEB\x03\x49\x8B\xC4\x48\x89\x87\x00\x00\x00\x00", "x????xxxxxxxx????");
	inline CSignature Location("\x40\x53\x48\x83\xEC\x20\x4C\x8B\xC2\x48\x8B\xD9\xBA\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x03\x33\xC0\x48\x89\x43\x28\x48\x89\x43\x30\x48\x89\x43\x38\x48\x8B\xC3", "xxxxxxxxxxxxx????x????xxx????xxxxxxxxxxxxxxxxxxxx", false);
	inline CSignature Water("\xE8\x00\x00\x00\x00\x48\x8B\xF8\x48\x89\x45\xA0\x48\x8B\x85\x00\x00\x00\x00\x0F\x10\x00\x0F\x11\x47\x28\xF2\x0F\x10\x48\x00\xF2\x0F\x11\x4F\x00\x8B\x40\x18\x89\x47\x40\x48\x8D\x15\x00\x00\x00\x00", "x????xxxxxxxxxx????xxxxxxxxxxx?xxxx?xxxxxxxxx????");
	inline CSignature Enemy("\x40\x53\x48\x83\xEC\x20\x4C\x8B\xC2\x48\x8B\xD9\xBA\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x45\x33\xC0", "xxxxxxxxxxxxx????x????xxx");
	inline CSignature Joint("\xE8\x00\x00\x00\x00\x48\x8B\xD8\x45\x33\xF6", "x????xxxxxx");
	inline CSignature Vehicle("\xE9\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x85\xC0\x74\x6D", "x????x????x????xxxxx");
	inline CSignature Wheel("\xE9\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x85\xC0\x74\x4E", "x????x????x????xxxxx");
	inline CSignature Screen("\xE8\x00\x00\x00\x00\x48\x8B\xF8\x48\x89\x45\xA0\xBB\x00\x00\x00\x00", "x????xxxxxxxx????");
	inline CSignature Trigger("\xE9\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x85\xC0\x74\x10", "x????x????x????xxxxx");
	inline CSignature Script("\xE8\x00\x00\x00\x00\x48\x8B\xF8\xEB\x03\x48\x8B\xFB", "x????xxxxxxxx");

	/*
		entity sigs
	*/
	inline CSignature GenVoxTexture("\xE8\x00\x00\x00\x00\x41\x0F\x10\x46\x00", "x????xxxx?");
	inline CSignature GenVoxPhysBuffer("\xE8\x00\x00\x00\x00\x41\xFF\xC7\x48\x83\xC7\x1C", "x????xxxxxxx");
	inline CSignature LoadVox("\xE8\x00\x00\x00\x00\x49\x89\x86\x00\x00\x00\x00", "x????xxx????");

	inline CSignature InitializeBody("\xE8\x00\x00\x00\x00\x48\x85\xF6\x74\x2B", "x????xxxxx");
	inline CSignature SetBodyDynamic("\x88\x91\x00\x00\x00\x00\x4C\x8B\xC1", "xx????xxx", false);

	/*
		registry sigs
	*/
	inline CSignature SetBool("\xE8\x00\x00\x00\x00\x90\x48\x8D\x4D\x87\xE8\x00\x00\x00\x00\x84\xDB", "x????xxxxxx????xx");
	inline CSignature SetInt("\xE8\x00\x00\x00\x00\x90\x48\x8D\x4D\xE8\xE8\x00\x00\x00\x00\x33\xD2", "x????xxxxxx????xx");
	inline CSignature SetFloat("\xE8\x00\x00\x00\x00\x48\x8D\x4D\x50", "x????xxxx");
	inline CSignature SetString("\xE8\x00\x00\x00\x00\x90\xEB\x1F", "x????xxx");

	inline CSignature GetBool("\xE8\x00\x00\x00\x00\x84\xC0\x74\x05\x40\xB6\x01", "x????xxxxxxx");
	inline CSignature GetInt("\xE8\x00\x00\x00\x00\x89\x43\x2C", "x????xxx");
	inline CSignature GetFloat("\xE8\x00\x00\x00\x00\xF3\x0F\x59\xF8", "x????xxxx");
	inline CSignature GetString("\x40\x53\x48\x83\xEC\x30\x49\x8B\xC0", "xxxxxxxxx", false);

	/*
		misc sigs
	*/
	inline CSignature Log("\x80\x79\x0F\x00\x74\x03\x48\x8B\x09\x48\x8B\xD1\x48\x8D\x0D\x00\x00\x00\x00", "xxxxxxxxxxxxxxx????", false);
	inline CSignature IsWindowActive("\xE8\x00\x00\x00\x00\x84\xC0\x74\x16\x48\x8B\xD3", "x????xxxxxxx");
	inline CSignature SetCursorCaptured("\x48\x83\xEC\x28\x48\x8B\x41\x08\x38\x50\x23", "xxxxxxxxxxx", false);
	inline CSignature QueryRaycast("\xE8\x00\x00\x00\x00\x84\xC0\x75\x5A", "x????xxxx");
}