#pragma once
#include "Interactor.h";

int main(int argc, char* argv[]) {
	if (argc == 1) {
		Interactor();
	}
	else if(argc == 2){
		Interactor(argv[1], argv[1]);
	}
	else if (argc == 3) {
		Interactor(argv[1], argv[2]);
	}
	else {
		std::cout << "��������! ��ȷ������ʽΪ[�����ļ���] [����ļ���(��ѡ)]";
		return 0;
	}
	
	return 0;
}