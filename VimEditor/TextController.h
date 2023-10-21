#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>

struct Buffer {
	char data[1024];
	Buffer* next;
	Buffer* prev;
};

class TextController {
private:
	const char* originText;
	std::ifstream inputStream;
	std::fstream outputStream;
	int charCount;
	int lineCount;
	int atLine;
	bool saved;
	Buffer* head;
	Buffer* tail;
	Buffer* at;

public:
	TextController(const char inputFile[], const char outputFile[]);
	~TextController();

	bool isSaved();
	int getLineNum();
	char* getLine();

	void insert(int line, char data[]);
	void jump(int line);
	void remove(int line);
	void print();
	void save();
	void quit();
};