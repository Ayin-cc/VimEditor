#pragma once
#include <iostream>

struct Buffer {
	char* data;
	Buffer* next;
	Buffer* prev;
};

class TextController {
private:
	const char* originText;
	const char* inputFile;
	const char* outputFile;
	Buffer* head;
	Buffer* tail;

public:
	TextController(const char inputFile[], const char outputFile[] = nullptr);
	~TextController();
	void append(char data[]);
	void insert(int line, char data[]);
	void jump(int line);
	void remove(int line);
	void print();

};

