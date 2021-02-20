#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#define REQ_CREATE	"CREATE"
#define REQ_REMOVE	"REMOVE"
#define REQ_APPEND	"APPEND"
#define REQ_GET 	"GET"

#define MAX_REQ_LEN			1000
#define BUFFER_LEN			4000
#define MAX_QUEUE_LEN 		2

#define COD_OK_4_OFILE		"OK-4 File name OK...opening file\n"
#define COD_OK_5_CFILE		"OK-5 File created successfully\n"
#define COD_OK_6_RFILE		"OK-6 File deleted successfully\n"
#define COD_OK_7_AFILE		"OK-7 File updated successfully\n"
#define COD_OK_1_CREATE		"OK-1 Mehtod CREATE OK\n"
#define COD_OK_2_REMOVE		"OK-2 Mehtod REMOVE OK\n"
#define COD_OK_3_APPEND		"OK-3 Mehtod APPEND OK\n"
#define COD_OK_0_GET		"OK-0 Mehtod GET OK\n"

#define COD_ERROR_2_CFILE	"ERROR-2 File could not be created\n"
#define COD_ERROR_3_RFILE	"ERROR-3 File could not be deleted\n"
#define COD_ERROR_4_AFILE	"ERROR-4 File could not be updated\n"
#define COD_ERROR_1_OFILE	"ERROR-1 File could not be opened\n"
#define COD_ERROR_0_METHOD	"ERROR-0 Method not supported\n"

int thread_cont = 0;

pthread_mutex_t op;

typedef struct req_t{
	char method[128];
	char filename[256];
	char content[512];
}req_t;

typedef struct arg_thread{
	int sockac;
	struct sockaddr_in cli_addr;
}arg_thread;

void get_request(req_t* req, char *req_str){
	bzero(req, sizeof(req_t));

	char *ptr = strstr(req_str, "APPEND");
	if(ptr != NULL && ptr - req_str == 0){
		sscanf(req_str, "%s \" %[^\"] \" %s", req->method, req->content, req->filename);
	}else{
		sscanf(req_str, "%s %s", req->method, req->filename);
	}
}

void get_file(int sockfd, req_t req){
	int ofile = open(req.filename, O_RDONLY, S_IRUSR);
	if(ofile == -1){
		perror("open");
		send(sockfd, COD_ERROR_1_OFILE, strlen(COD_ERROR_1_OFILE), 0);
		return;
	}

	send(sockfd, COD_OK_4_OFILE, strlen(COD_OK_4_OFILE), 0);
	send(sockfd, "\nFILE CONTENT:\n", strlen("\nFILE CONTENT:\n"), 0);
	
	int nr;
	unsigned char buffer[BUFFER_LEN];
	do{
		bzero(buffer, BUFFER_LEN);
		nr = read(ofile, buffer, BUFFER_LEN);
		if (nr > 0) {
			send(sockfd, buffer, nr, 0);
		}
	}while (nr > 0);

	send(sockfd, "\n", strlen("\n"), 0);

	close(ofile);
	return;
}

void create_file(int sockfd, req_t req){
	int cfile = creat(req.filename, 0777);
	if(cfile == -1){
		perror("create");
		send(sockfd, COD_ERROR_2_CFILE, strlen(COD_ERROR_2_CFILE), 0);
		return;
	}

	send(sockfd, COD_OK_5_CFILE, strlen(COD_OK_5_CFILE), 0);
	
	close(cfile);
	return;
}

void remove_file(int sockfd, req_t req){	
	int rfile = remove(req.filename);
	if(rfile == -1){
		perror("remove");
		send(sockfd, COD_ERROR_3_RFILE, strlen(COD_ERROR_3_RFILE), 0);
		return;
	}

	send(sockfd, COD_OK_6_RFILE, strlen(COD_OK_6_RFILE), 0);
	
	close(rfile);
	return;
}

void append_file(int sockfd, req_t req) {
	int afile = open(req.filename, O_WRONLY | O_APPEND, 0777);
	if(afile == -1) {
		perror("append");
		send(sockfd, COD_ERROR_4_AFILE, strlen(COD_ERROR_4_AFILE), 0);
		return;
	}

	send(sockfd, COD_OK_7_AFILE, strlen(COD_OK_7_AFILE), 0);

	write(afile, req.content, strlen(req.content));
	write(afile, " \n", strlen(" \n"));

	close(afile);
	return;
}

void proc_request(int sockfd, req_t req){
	if(strcmp(req.method, REQ_GET) == 0){
		send(sockfd, COD_OK_0_GET, strlen(COD_OK_0_GET), 0);
		get_file(sockfd, req);

	}else if(strcmp(req.method, REQ_CREATE) == 0){
		send(sockfd, COD_OK_1_CREATE, strlen(COD_OK_1_CREATE), 0);
		create_file(sockfd, req);

	}else if(strcmp(req.method, REQ_REMOVE) == 0){
		send(sockfd, COD_OK_2_REMOVE, strlen(COD_OK_2_REMOVE), 0);
		remove_file(sockfd, req);

	}else if(strcmp(req.method, REQ_APPEND) == 0){
		send(sockfd, COD_OK_3_APPEND, strlen(COD_OK_3_APPEND), 0);
		append_file(sockfd, req);

	}else{
		send(sockfd, COD_ERROR_0_METHOD, strlen(COD_ERROR_0_METHOD), 0);
	}
	return;
}

void* recv_request(void* arg){
	arg_thread *args_ptr = (arg_thread *) arg;
	arg_thread args = *args_ptr;

	char request[MAX_REQ_LEN];
	req_t req;

	if(args.sockac == -1){
		perror("accept");

		pthread_mutex_lock(&op);
		thread_cont -= 1;
		pthread_mutex_unlock(&op);

		pthread_exit(0);
	}

	printf("Connected to client %s:%d\n\n", inet_ntoa(args.cli_addr.sin_addr), ntohs(args.cli_addr.sin_port));
	
	bzero(request, MAX_REQ_LEN);
	int nbytes_recv = recv(args.sockac, request, MAX_REQ_LEN, 0);
	
	if(nbytes_recv > 0){
		get_request(&req, request);
		printf("Request from %s:%d\n Method: %s\n Filename: %s\n", inet_ntoa(args.cli_addr.sin_addr), ntohs(args.cli_addr.sin_port), req.method, req.filename);
		
		if(strcmp(req.method, "APPEND") == 0){
			printf(" Content: %s\n", req.content);
		}
		printf("\n");

		proc_request(args.sockac, req);
	}

	send(args.sockac, "\n", strlen("\n"), 0);
	close(args.sockac);

	pthread_mutex_lock(&op);
	thread_cont -= 1;
	pthread_mutex_unlock(&op);

	pthread_exit(0);
}


int main(int argc, char **argv){
	pthread_mutex_init(&op, NULL);

	//checking input argument
	if(argc != 2){
		printf("How to run: %s <port>\n", argv[0]);
		return 0;
	}

	//socket file descriptor
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sockfd == -1){
		perror("socket");
		return 0;
	}
	
	//server address
	struct sockaddr_in serv_addr;
	serv_addr.sin_port = htons(atoi(argv[1]));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	//assigns address to the socket
	if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) == -1){
		perror("bind");
		return 0;
	}

	//socket listening to a max of MAX_QUEUE_LEN requests
	if(listen(sockfd, MAX_QUEUE_LEN) == -1){
		perror("listen");
		return 0;
	}
	
	struct sockaddr_in cli_addr;
	int addr_len;
	int sockac;
	
	while(1){
		addr_len = sizeof(struct sockaddr_in);
		
		if(thread_cont < MAX_QUEUE_LEN){
			sockac = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&addr_len);
		
			pthread_mutex_lock(&op);
			thread_cont += 1;
			pthread_mutex_unlock(&op);

			arg_thread args;
			args.cli_addr = cli_addr;
			args.sockac = sockac;
			pthread_t thread;

			pthread_create(&thread, NULL, recv_request, (void *)&args);
		}
	}

	close(sockfd);
	return 0;
}
