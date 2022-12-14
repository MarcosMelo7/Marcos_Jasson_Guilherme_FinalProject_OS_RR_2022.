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

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

// Verifica se o socket foi criado correntamente, caso contrario, imprime o erro
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-] Erro na conexão.\n");
		exit(1);
	}
	printf("[+] Socket do cliente criado!.\n");

// Configuração para estabelecer conexao (endereco)
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

// Verifica se a conexao foi feita com sucesso, caso contrário, imprime o erro
	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-] Erro na conexão.\n");
		exit(1);
	}
	printf("[+] Conectado no servidor.\n");

	// Diálogo entre servidor e cliente
	while(1){
		// Cliente digita a mensagem e envia ao servidor
		printf("Client: \t");
		scanf("%s", &buffer[0]);
		send(clientSocket, buffer, strlen(buffer), 0);

		// Analisa se foi solicitado encerramento da conexao
		if(strcmp(buffer, ":exit") == 0){
			close(clientSocket);
			printf("[-] Conexão encerrada com o servidor.\n");
			exit(1);
		}

		if(recv(clientSocket, buffer, 1024, 0) < 0){
			printf("[-] Erro no recebimento de dados.\n");
		}else{
			printf("Server: \t%s\n", buffer);
		}
	}

	return 0;
}
