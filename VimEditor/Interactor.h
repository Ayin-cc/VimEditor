#pragma once

#include "TextController.h"
#include "Help.h"

class Interactor {
private:
	const char PROMPT = ':';
	const char ORDER[15][7] = {
		"help",
		"v",
		"begin",
		"end",
		"p",
		"n",
		"j",
		"insert",
		"d",
		"r",
		"f",
		"info",
		"reload",
		"wq",
		"q"
	};
	TextController* controller;

public:
	Interactor();
	Interactor(const char inputFile[], const char outputFile[] = nullptr);
	~Interactor();
};

