#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <semaphore.h>
sem_t semaphore1;
sem_t semaphore2;
bool isVowel(char a) {
	a = tolower(a);
	if (a == 'a' || a == 'e' || a == 'i' ||
		a == 'o' || a == 'u' || a == 'y') {
		return true;
	}
	return false;
}
char* stringToString(char* stream) {
	unsigned int maxLength = 64, size = 64;
	char* buffer = (char*)malloc(maxLength);
	if (buffer != NULL) {
		char character = EOF;
		int index = 0;
		while ((character = *(stream + index)) != '\n' && character != EOF) {
			buffer[index] = character;
			index++;
			if (index == size) {
				size = index + maxLength;
				buffer = (char*)realloc(buffer, size);
			}
		}
		buffer[index] = '\0'; //терминальный нуль -для обозначения конца строки
	}
	return buffer;
}
int main() {
	std::fstream fos1, fos2;
	int fileDescriptor;
	struct stat statusBuffer;
	if (sem_init(&semaphore1, 1, 1) < 0) {
		std::cout << "SEM1 ERROR" << std::endl;
		exit(-1);
	}
	if (sem_init(&semaphore2, 1, 1) < 0) {
		std::cout << "SEM2 ERROR" << std::endl;
		exit(-1);
	}
	if ((fileDescriptor = open("test.txt", O_RDWR)) < 0) {
		std::cout << "SRC FILE OPEN ERROR" << std::endl;
		exit(-1);
	}
	char* source;
	if (fstat(fileDescriptor, &statusBuffer) < 0) {
		std::cout << "FSTAT ERROR" << std::endl;
		exit(-1);
	}
	source = (char*)mmap(NULL, statusBuffer.st_size, PROT_READ, MAP_SHARED,
		fileDescriptor, 0);
	if (source == MAP_FAILED) {
		std::cout << "MAPPING ERROR" << std::endl;
		exit(-1);
	}
	int child1, child2;
	if ((child1 = fork()) == -1) {
		std::cout << "FORK1 ERROR" << std::endl;
		exit(-1);
	}
	else if (child1 == 0) {
		sem_wait(&semaphore1);
		fos1.open("1.txt", std::fstream::out);
		int index = 0;
		char* string;
		while ((string = stringToString(source + index)) && string[0] != '\0') {
			int lenght = strlen(string);
			if (lenght <= 10) {
				for (int i = 0; i < lenght; i++) {
					if (!isVowel(string[i])) {
						fos1 << string[i];
					}
				}
				fos1 << '\n';
			}
			index += lenght;
			if (source[index] == '\n') index++;
			free(string);
		}
		free(string);
		sem_post(&semaphore1);
	}
	else {
		if ((child2 = fork()) == -1) {
			std::cout << "FORK2 ERROR" << std::endl;
			exit(-1);
		}
		else if (child2 == 0) {
			sem_wait(&semaphore2);
			fos2.open("2.txt", std::fstream::out);
			int index = 0;
			char* string;
			while ((string = stringToString(source + index)) && string[0] != '\0'){
				int length = strlen(string);
				if (length > 10) {
					for (int i = 0; i < length; i++) {
						if (!isVowel(string[i])) {
							fos2 << string[i];
						}
					}
					fos2 << '\n';
				}
				index += strlen(string);
				if (source[index] == '\n') index++;
				free(string);
			}
			free(string);
			sem_post(&semaphore2);
		}
		else {
			sem_wait(&semaphore1);
			sem_wait(&semaphore2);
			close(fileDescriptor);
			if (munmap(source, statusBuffer.st_size) < 0) {
				std::cout << "UNMAPPING ERROR" << std::endl;
				exit(-1);
			}
			if (sem_destroy(&semaphore1) < 0) {
				std::cout << "SEMDEL1 ERROR" << std::endl;
				exit(-1);
			}
			if (sem_destroy(&semaphore2) < 0) {
				std::cout << "SEMDEL2 ERROR" << std::endl;
				exit(-1);
			}
		}
	}
}


