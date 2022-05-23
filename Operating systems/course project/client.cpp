#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <vector>
#include <fcntl.h>
#include "funcs.hpp"
#include <thread>
#include <string>

//функция приёма сообщений (для потока)
void func(int fd_recv, std::string login)
{
    while (1)
    {
        std::string reply = recieve_message(fd_recv);
        std::cout << reply << "\n";
        std::cout.flush();
        std::cout << login << ">";
        std::cout.flush();
    }
}

//функция получения адреса для вывода черновика
std::string rec(std::string s){
	std::string str;
	for(int i = 0; i < s.length()-1; i++){
		str.push_back(s[i+1]);
	}
	return str;
}


int main()
{
    //подключение к входному FIFO сервера
    int fd_send = open("input", O_RDWR);
    if (fd_send == -1)
    {
        std::cout << "ERROR: MAIN FIFO WAS NOT OPENED\n";
        exit(1);
    }

    //подготовка - инструкции, ввод логина
    std::cout << "Hello. Insert logins.\n If you stop the enter logins you can enter 'end'\n";
    std::cout << "Input: [login] [message]. for example like : one\n hey, how are you?\n";
    std::cout << "Insert your login: ";
    std::string login;
    std::vector<std::string> drafts;

    //подключение к персональному именованному пайпу
    int fd_recv = -1;
    while (fd_recv == -1)
    {
        std::cin >> login;
        fd_recv = open(login.c_str(), O_RDWR);
        if (fd_recv == -1){
            std::cout << "Wrong login!\nInsert your login: ";
        }
    };

    //вход успешен, запуск потока принятия сообщений от сервера
    std::string adressee, message;
    std::cout << "Congrats! You have signed. Now you can send messages!\n";
    std::thread thr_recieve(func, fd_recv, login);

    //запуск цикла отправки сообщений на сервер
    while (1){
        std::cout << login << "> ";
        std::cin >> adressee;
        if (adressee == "draft"){
            std::string mesage;
            std::getline(std::cin, mesage);
            drafts.push_back(mesage);
        } 
        else{
        if (adressee == "send_draft"){
            std::string recipient;
            std::getline(std::cin,recipient);
	    std::string recipien = rec(recipient);
	    //std::cout<<"we there!!drafts.size()="<<drafts.size()<<std::endl;
            for(int i = 0; i < drafts.size(); i++){
         	//std::cout<<"i= "<<i<<" drafts[i] = "<<drafts[i]<<std::endl;
		std::cout<<"adr= "<<recipien<<std::endl;
                send_message_on_server(fd_send, login, recipien, drafts[i]); //login, drafts[i]);//recipient, drafts[i]);
            }
        }
        else{
            if (adressee == "quit"){
		for(int i = 0; i < drafts.size(); i++){
			send_message_on_server(fd_send, login, login, drafts[i]);
		}
                break;
	    }
            std::getline(std::cin, message);
	    //std::cout<<"adress= "<<adressee<<std::endl;
            send_message_on_server(fd_send, login, adressee, message);
        }
   
       	}
    }

    thr_recieve.detach();
}
