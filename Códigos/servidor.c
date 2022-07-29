#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4950

int main(){
	// Declaracao das variáveis utilizadas no processo
	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-] Erro na conexão.\n");
		exit(1);
	}
	printf("[+] Socket do cliente criado!\n");

	// Definição e configuração de parametros de enderecamento
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// Associa o socket com o endereco local
	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-] Erro na vinculação. \n");
		exit(1);
	}
	printf("[+] Vinculado a porta %d\n", 4950);

	// Imprime uma mensagem enquanto não há cliente conectado
	if(listen(sockfd, 10) == 0){
		printf("[+] Fazendo a leitura....\n");
	}else{
		printf("[-] Erro na vinculação. \n");
	}

	// Verifica e aceita a conexão com o cliente
	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Conexão aceita com %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);
			// Verifica a mensagem enviada, caso o cliente tenha desconectado ele imprime o evento que desconectou, se não ele imprime a msg.
			while(1){
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, ":exit") == 0){
					printf("Desconectado com %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					printf("Cliente: %s\n", buffer);
					send(newSocket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
				}
			}
		}

	}

	// Encerra conexão com este cliente
	close(newSocket);
	printf("A conexao foi encerrada.\n");

	return 0;
}
