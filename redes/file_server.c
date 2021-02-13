#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#define REQ_GET "GET"
#define REQ_CREATE "CREATE"
#define REQ_REMOVE "REMOVE"
#define REQ_APPEND "APPEND"

#define MAX_QUEUE_LEN 1000
#define MAX_REQ_LEN 1000
#define BUFFER_LEN 4000

#define COD_OK_0_GET "OK-0 Mehtod GET OK\n"
#define COD_OK_1_CREATE "OK-1 Mehtod CREATE OK\n"
#define COD_OK_2_REMOVE "OK-2 Mehtod REMOVE OK\n"
#define COD_OK_3_APPEND "OK-3 Mehtod APPEND OK\n"
#define COD_OK_4_OFILE "OK-4 File name OK...opening file\n"
#define COD_OK_5_CFILE "OK-5 File created successfully\n"
#define COD_OK_6_RFILE "OK-6 File deleted successfully\n"
#define COD_OK_7_AFILE "OK-7 File updated successfully\n"

#define COD_ERROR_0_METHOD "ERROR-0 Method not supported\n"
#define COD_ERROR_1_OFILE "ERROR-1 File could not be opened\n"
#define COD_ERROR_2_CFILE "ERROR-2 File could not be created\n"
#define COD_ERROR_3_RFILE "ERROR-3 File could not be deleted\n"
#define COD_ERROR_4_AFILE "ERROR-4 File could not be updated\n"


typedef struct req_t {
	char method[128];
	char filename[256];
} req_t;

typedef struct arg_thread {
	int sockac;
	struct sockaddr_in cli_addr;
} arg_thread;


void get_request(req_t* req, char *req_str){
	bzero(req, sizeof(req_t));
	sscanf(req_str, "%s %s", req->method, req->filename);
}

void send_file(int sockfd, req_t req) {
	int ofile, nr;
	unsigned char buffer[BUFFER_LEN];
	ofile = open(req.filename, O_RDONLY, S_IRUSR);
	if (ofile == -1) {
		perror("open");
		send(sockfd, COD_ERROR_1_OFILE, strlen(COD_ERROR_1_OFILE), 0);
		return;
	}
	send(sockfd, COD_OK_4_OFILE, strlen(COD_OK_4_OFILE), 0);
	 
	do {
		bzero(buffer, BUFFER_LEN);
		nr = read(ofile, buffer, BUFFER_LEN);
		if (nr > 0) {
			send(sockfd, buffer, nr, 0);
		}
	} while (nr > 0);
	close(ofile);
	return;
}

void create_file(int sockfd, req_t req) {
	int cfile;
	cfile = creat(req.filename, S_IWUSR);
	if (cfile == -1) {
		perror("create");
		send(sockfd, COD_ERROR_2_CFILE, strlen(COD_ERROR_2_CFILE), 0);
		return;
	}
	send(sockfd, COD_OK_5_CFILE, strlen(COD_OK_5_CFILE), 0);
	close(cfile);
	return;
}

void remove_file(int sockfd, req_t req) {
	int rfile;
	rfile = remove(req.filename);
	if (rfile == -1) {
		perror("remove");
		send(sockfd, COD_ERROR_3_RFILE, strlen(COD_ERROR_3_RFILE), 0);
		return;
	}
	send(sockfd, COD_OK_6_RFILE, strlen(COD_OK_6_RFILE), 0);
	close(rfile);
	return;
}

void update_file(int sockfd, req_t req) {
}

void proc_request(int sockfd, req_t req){
	if (strcmp(req.method, REQ_GET) == 0) {
		send(sockfd, COD_OK_0_GET, strlen(COD_OK_0_GET), 0);
		send_file(sockfd, req);
	} else if (strcmp(req.method, REQ_CREATE) == 0) {
		send(sockfd, COD_OK_1_CREATE, strlen(COD_OK_1_CREATE), 0);
		create_file(sockfd, req);
	} else if (strcmp(req.method, REQ_REMOVE) == 0) {
		send(sockfd, COD_OK_2_REMOVE, strlen(COD_OK_2_REMOVE), 0);
		remove_file(sockfd, req);
	} else if (strcmp(req.method, REQ_APPEND) == 0) {
		send(sockfd, COD_OK_3_APPEND, strlen(COD_OK_3_APPEND), 0);
		//update_file
	} else {
		send(sockfd, COD_ERROR_0_METHOD, strlen(COD_ERROR_0_METHOD), 0);
	}
	return;
}

void* recv_request(void* arg) {
	arg_thread *args_ptr = (arg_thread *) arg;
	arg_thread args = *args_ptr;

	char request[MAX_REQ_LEN];
	req_t req;

	if (args.sockac == -1){
		perror("accept");
		pthread_exit(0);
	}
	printf("Conected to client %s:%d\n", inet_ntoa(args.cli_addr.sin_addr), ntohs(args.cli_addr.sin_port));
	bzero(request, MAX_REQ_LEN);
	//ssize_t nbytes_recv
	int nbytes_recv = recv(args.sockac, request, MAX_REQ_LEN, 0);
	if (nbytes_recv > 0){
		get_request(&req, request);
		printf("Request from %s:%d\n method: %s\n filename: %s\n", inet_ntoa(args.cli_addr.sin_addr), ntohs(args.cli_addr.sin_port), req.method, req.filename);
		proc_request(args.sockac, req);
	}
	close(args.sockac);
	pthread_exit(0);
}


int main(int argc, char **argv){
	//checking input argument
	if (argc != 2){
		printf("How to run: %s <port>\n", argv[0]);
		return 0;
	}
	//socket file descriptor
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd == -1){
		perror("socket");
		return 0;
	}
	//server address
	struct sockaddr_in serv_addr;
	serv_addr.sin_port = htons(atoi(argv[1]));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	//assigns address to the socket
	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) == -1){
		perror("bind");
		return 0;
	}
	//socket listening to a max of MAX_QUEUE_LEN requests
	if (listen(sockfd, MAX_QUEUE_LEN) == -1){
		perror("listen");
		return 0;
	}
	
	struct sockaddr_in cli_addr;
	int addr_len;
	int sockac;
	
	pthread_t thread[MAX_QUEUE_LEN];
	int cont = -1;
	while(1) {
		cont++;
		addr_len = sizeof(struct sockaddr_in);
		sockac = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&addr_len);
		
		arg_thread args;
		args.cli_addr = cli_addr;
		args.sockac = sockac;
		
		pthread_create(&thread[cont], NULL, recv_request, (void *)&args);
		
		//tratar cont para usar posições que foram liberadas
	}
	close(sockfd);
	return 0;
}
