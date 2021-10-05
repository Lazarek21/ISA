CC = g++
CFLAGS = -g -Wall -pedantic

build:
	clear
	@echo "Build.."
	$(CC) $(CFLAGS) main.cpp -o secret
	@echo "Success.."
	
check:
	@echo "Start testing.."
	@echo "\n---Empty start"
	./secret
	@echo "\n---Listen start"
	./secret -l
	@echo "\n---Listen start"
	./secret -L
	@echo "\n---Sender start"
	./secret -r cesta.txt -s 192.168.1.21
	@echo "\n---Listen start invalid parameter"
	./secret -l
	@echo "\n---Sender start invalid first parameter"
	./secret -f cesta.txt -s 192.168.1.21
	@echo "\n---Sender start invalid second parameter"
	./secret -r cesta.txt -x 192.168.1.21
	@echo "\n---Sender start invalid both parameter"
	./secret -d cesta.txt -a 192.168.1.21
	@echo "\nStop testing.."