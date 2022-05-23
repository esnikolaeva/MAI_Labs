#include <iostream>
#include <string>
#include <fstream>
#include "unistd.h"

int main(){
	std::string th_child1;
	std::string th_child2;
	std::cout << "This is parent process" << std::endl;
	std::cout << "Enter names for first and second childs" << std::endl;
	std::cin >> th_child1;
	std::cin >> th_child2;
	std::fstream fs;
	int fd1[2];
	pipe(fd1);
	
	int fd2[2];
	pipe(fd2);
	
	if (pipe(fd1) == -1){
		std::cout << "Error" << std::endl;
		return 1;
	}
	if (pipe(fd2) == -1){
		std::cout << "Error" << std::endl;
		return 1;
	}
	int first_id = fork();
	if (first_id == -1){
		std::cout << "Error" << std::endl;
		return -1;
	}
	else if (first_id == 0){ //work with 1 child (when <= 10 words)
		fs.open(th_child1, std:: fstream:: in | std:: fstream:: out | std:: fstream:: app);
	int a = 0;
	read(fd1[0], &a, sizeof(int));
	std::cout << "Your in child 1 process" << std::endl;
	while(a > 0){
		int size;
		read(fd1[0], &size, sizeof(int));
		char array[size];
		read(fd1[0], array, sizeof(char)*size);
		std::string string;
		for (int i = 0; i < size; i++){
			string.push_back(array[i]);
		}
		for(int i = 0; i < size/2; i++){ //invert
			char tmp = string[i];
			string[i] = string[size-i-1];
			string[size-i-1] = tmp;
		}
		fs << string << std::endl;
		std::cout<<"After work in child 1 your string look as: "<< string <<std::endl;
		a = a - 1;
	}
	close(fd1[0]);
	close(fd1[1]);
	}
	else {
		int second_id = fork();
		if (second_id == -1){
			std::cout<<"Error"<<std::endl;
			return -1;
		}
	else if (second_id == 0) { //work with 2 child (when > 10 words)
		fs.open(th_child1, std:: fstream:: in | std:: fstream:: out | std:: fstream:: app);
	int a;
	read(fd2[0], &a, sizeof(int));
	std::cout << "Your in child 2 process" << std::endl;
	while(a > 0) {
		int size;
		read(fd2[0], &size, sizeof(int));
		char array[size];
		read(fd2[0], array, sizeof(char)*size);
		std::string string;
		for (int i = 0; i<size; i++){
		string.push_back(array[i]);
	}
	for (int i = 0; i < size/2; i++){ //invert
		char tmp = string[i];
		string[i] = string[size-i-1];
		string[size-i-1] = tmp;
	}
	fs << string << std::endl;
	std::cout << "After work in child 2 your string look as: "<< string << std::endl;
	a = a - 1;
	close(fd2[0]);
	close(fd2[1]);
	}
	}
	else {
		int a;
		std::cout << "PARENT: Please, enter number of string" << std::endl;
		std::cin >> a;
		write(fd1[1], &a, sizeof(int));
		write(fd2[1], &a, sizeof(int));
		std::cout << "PARENT: Please, enter your strings" << a << "time" << std::endl; 
		for(int i = 0; i < a; i++){
			std::string string;
			std::cin >> string;
			int num = string.size();
			char array[num];
			for (int i = 0; i < num; i++){
				array[i] = string[i];
			}
			if (string.size() <= 10){
				write(fd1[1], &num, sizeof(int));
				write(fd1[1], array, sizeof(char)* num);
			}
			else{
				write(fd2[1], &num, sizeof(int));
				write(fd2[1], array, sizeof(char)* num);
			}
		}
		close(fd1[0]);
		close(fd1[1]);
		close(fd2[0]);
		close(fd2[1]);
	}
	return 0;
	}
}
