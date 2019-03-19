#include<stdio.h>  
#include<stdlib.h>  
#include<unistd.h>  
#include<string.h>  
#include<assert.h>  
#include<sys/socket.h>  
#include<netinet/in.h>  
#include<arpa/inet.h>  
#include<pthread.h>  

void *fun(void *arg)  //子线程，线程函数，读取数据
{
	int c  = (int)arg;
	//循环处理我们已建立连接的客户端  

}
int main()
{
	int sockfd  = socket(AF_INET, SOCK_STREAM, 0);
	//创建进程套接字 ，sockfd 为监听套接字， 被视作一个打开的文件，和文件一样返回一个文件描述符  
	//socket()函数中三参数依次为：ipv4 、 流式服务 、协议类型  
	assert(sockfd  != -1);
	struct sockaddr_in saddr, caddr;
	//定义套接字地址结构  sockaddr_in  专有   不带in  通用   
	memset(&saddr, 0, sizeof(saddr));//初始化，清空结构体（因为有占位）  

	saddr.sin_family  = AF_INET;
	saddr.sin_port  = htons(6000);//转大端     
	//端口值：1024以内为知名端口 root管理员可以使用  HTTP使用80端口；  
	//1024-4096  保留端口；4096以上  临时端口，应用程序使用的  
	saddr.sin_addr.s_addr  = inat_addr("127.0.0.1");//转成无符号整型  
	//以上三句为填充套接字结构成员变量操作  

	int res  = bind(sockfd, (struct sockaddr*)&saddr, sizeof(saddr));
	//将套接字与自己的IP绑定  (struct sockaddr*)&saddr:强转成通用  
	assert(res  != -1);
	listen(sockfd, 5);//等待客户端连接，创建监听队列，转变为可以被动监听的队列，5为监听队列的长度  
	while (1)
	{
		int len  = sizeof(caddr);//caddr  客户端信号  
		int c  = accept(sockfd, (struct sockaddr*)&caddr, &len);
		//c为连接套接字  没有人连接，会阻塞，它是处理已完成三次握手的队列的，返回新的描述符
		if (c < 0)
		{
			continue;
		}
		printf("accept c= %d ip= %s\n", c, inet_ntoa(caddr.sin_addr));
		pid_t pid = fork();
		assert(pid != -1);
		if (pid < 0)
		{
			close(c);
			continue;
		}
		if (pid == 0)
		{
			while (1)
			{
				char buff[128] = { 0 };
				int n  = recv(c, buff, 127, 0);
				if (n  <= 0)//对方close n为0  
				{
					break;
				}
				printf("buff(%d)=%s\n", c, buff);
				send(c, "ok", 2, 0);
			}
			printf("one client over\n");
			close(c);
		}
	}
}
