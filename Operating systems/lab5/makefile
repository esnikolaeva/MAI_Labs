сборка и запуска первой библиотеки:
g++ -fPIC -c library1.cpp -o l1.o
g++ -shared l1.o -o libd1.so
export LD_LIBRARY_PATH=/home/elizaveta/laba5/
sudo ldconfig
g++ main1.cpp -L/home/elizaveta/laba5/ -ld1 - o a
./a

сборка и запуск второй библиотеки:
g++ -fPIC -c library2.cpp -o l2.o
g++ -shared l2.o -o libd.so
g++ main2.cpp -ldl - o c
./c
