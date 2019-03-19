#include<stdio.h>  
#include<stdlib.h>  
#include<unistd.h>  
#include<string.h>  
#include<assert.h>  
#include<sys/socket.h>  
#include<netinet/in.h>  
#include<arpa/inet.h>  
#include<pthread.h>  


int main()
{
	int sockfd  = socket(AF_INET, SOCK_STREAM, 0);
	//sockfd 为连接套接字  
	assert(sockfd  != -1);
	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family  = AF_INET;
	saddr.sin_port  = htons(6000);
	saddr.sin_addr.s_addr  = inat_addr("127.0.0.1");

	int res  = connect(sockfd, (struct sockaddr*)&saddr, sizeof(saddr));
	//三次握手建立连接  
	assert(res  != -1);

	while (1)
	{
		char buff[128] = { 0 };
		printf("input:\n");
		fgets(buff, 128, stdin);//从键盘获取  
		if (strncmp(buff, "end", 3) == 0)
		{
			break;
		}
		send(sockfd, buff, strlen(buff), 0);//发给服务器端  
		memset(buff, 0, 128);
		recv(sockfd, buff, 127, 0);//接收对方回复的数据  
		printf("buff=%s\n", buff);
		close(sockfd);
	}
}
