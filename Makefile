all: ./server/WTFserver.c ./client/WTF.c
	gcc -pthread -o server/WTFserver ./server/WTFserver.c; gcc -o client/WTF ./client/WTF.c -lcrypto -lssl ;mkdir -p server/repository/backups

test: ./client/WTFtest.c
	gcc -pthread -o server/WTFserver ./server/WTFserver.c; gcc -o client/WTF ./client/WTF.c -lcrypto -lssl ;mkdir -p server/repository/backups

	gcc -pthread -o  client/WTFtest ./client/WTFtest.c
	./client/WTFtest

val-create: 
	valgrind --leak-check=full ./client/WTF create testProj
clean:
	rm ./server/WTFserver; rm ./client/WTF 
cleantest:
	rm ./server/WTFserver; rm ./client/WTF ; rm -r ./client/WTFtest ./server/repository/test  ./server/repository/backups/test ./client/test; rmdir ./client/WTFtest ; rm client/.configure ; rm ./client/.History
cleanrepo:
	rm ./server/WTFserver; rm ./client/WTF ; rm -r ./server/repository  ./client/WTFtest  ./client/test ; rm ./client/.History


a:
	make clean all
