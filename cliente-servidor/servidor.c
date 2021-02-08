#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netdb.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "mensagem.c"
//tamanho do buffer
#define TAM 10000000
//defique o socket de comunicação
int skt;

pthread_mutex_t meu_mutex;

void *servidor(void *thread);
int disparo();
int writing(int time, int value, int socket);
void *func_GUI(void *threads_GUI)
{
	int num = *(int *)threads_GUI;
	printf("Thread rodando %d \n", num);
	puts("chat disponivel");

	puts("mensagem a disparar");

	if (skt != 1)
	{
		while (1)
		{

			//disparo();
		}
	}
}
int soma(char **string, int length)
{
	int somatorio = 0;
	for (size_t i = 0; i < length; i++)
	{
		//somatorio += atoi(string[i]);
		puts(string[i]);
	}
	return somatorio;
}
void *reading(void *threads)
{
	sleep(2);
	clock_t t;
	t = clock();
	int somatorio = 0, rd,i=0;
	int socket = *(int *)threads;
	char **string = AlocaMatriz(1000, 5);
	char aux[5];
	while (1)
	{
		if (rd = read(socket, aux, 5) > 0)
		{
			
				if (strcmp(aux, "-1") != 0)
				{
					string[i] = aux;

					//printf("recebido: %s", string[i]);
					somatorio += atoi(string[i]);
					i++;
				}
				else
				{
					
					t = clock() - t;
					writing(t, somatorio, socket);
					break;

				}
			
		}
	}
}
int writing(int time, int value, int socket)
{
	
	char MsgRetorno[1000] = "Tempo de calculo: ";
	char aux[100];
	sprintf(aux, "%d", time);
	
	strcat(MsgRetorno, aux);
	strcat(MsgRetorno, " com o resultado: ");
		
	sprintf(aux, "%d", value);
	strcat(MsgRetorno, aux);
	puts("enviando resposta");
	int _Write = write(socket, MsgRetorno, 1000);
	if (_Write == -1)
	{
		puts("Error ao enviar");
	}

	puts("Enviado");
	
	return 0;
}

int main(int argc, char const *argv[])
{

	pthread_t threads_GUI, thread_read;
	pthread_t threads_server;
	// pthread_mutex_init(&meu_mutex, NULL );
	int id = 1, id2 = 2;
	pthread_create(&threads_server, NULL, servidor, (void *)&id2);
	pthread_join(threads_server, NULL);
	pthread_create(&threads_GUI, NULL, func_GUI, (void *)&id);
	//pthread_create(&thread_read,NULL,reading,(void*)&id);

	//pthread_create(&threads_GUI[1],NULL,func_comunicacao,(void*)&id[1]);

	pthread_join(threads_GUI, NULL);
	pthread_join(thread_read, NULL);

	return 0;
}

void *servidor(void *thread)
{
	// definindo o socket de comunicação tcp em ipv4
	int socket_com = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_com == -1)
	{
		printf("Erro na criacao do socket \n");
		exit(1);
	}
	else
		printf("Sucesso!\n");
	// definição do bind para definição da estrutura de comunicação( portas e ip);
	struct sockaddr_in server;
	struct sockaddr_in cliente;

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5555);

	if (bind(socket_com, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf("Erro ao fazer bind\n");
		exit(1);
	}
	puts("Bind feito com sucesso!");

	//definindo a liste ( liberação da porta para conexões tcp )

	int status = listen(socket_com, 10);
	if (status != 0)
	{
		perror("Erro na chamada listen()");
		exit(1);
	}
	//printf("numero de requisicoes:%d\n",status);

	int size = sizeof(cliente);
	while (1)
	{
		printf("aguardando conexão\n");
		int socket_cliente = accept(socket_com, (struct sockaddr *)&cliente, (socklen_t *)&size);

		if (socket_cliente < 0)
		{
			puts("erro na comunicacao acept");
			exit(1);
		}
		else
		{
			printf("socket_cliente: %d\n", socket_cliente);
			pthread_t thread_read;
			pthread_create(&thread_read, NULL, reading, (void *)&socket_cliente);
			pthread_join(thread_read, NULL);
		}
		//else skt= socket_cliente;

		//mensagem de boas vindas

		//printf("socket_cliente: %d\n", socket_cliente);
	}

	return 0;
}
