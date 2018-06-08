    #include <stdio.h>  
    #include <stdlib.h>  
    #include <sys/types.h>  
    #include <sys/stat.h>  
    #include <fcntl.h>  
    #include <sys/wait.h>  
    #include <unistd.h>  
    #include <string.h>  
    #include <pwd.h>  
      
    //获取当前用户名   
    void GetLoginname()  
    {  
        struct passwd *pwd=getpwuid(getuid());  
        printf("[%s",pwd->pw_name);  
    }  
      
    //获取当前主机名   
    void GetHostname()  
    {  
        char name[256]={0};  
        int ret=gethostname(name,256);  
        if(ret==0)  
            printf("@%s",name);  
    }  
      
    //获取当前路径   
    void GetPwd()  
    {  
        char pwd[256]={0};  
        getcwd(pwd,sizeof(pwd)-1);//pwd存储的为相对路径   
        int len=strlen(pwd);  
        //用p获取当前路径   
        char* p=pwd+len;  
        while(*p!='/'&&len--)  
            --p;  
        ++p;  
        printf(" %s]#",p);  
    }  
    
    //主进程  
    void Myshell(){  
        //shell死循环实现4   
        while(1){   
            //打印shell前面的那几个字   
            GetLoginname();  
            GetHostname();  
            GetPwd();  
            fflush(stdout);   
	    //刷新缓冲区以便不影响后面读取   
                 
            //读字符串到缓冲区中   
            char buf[1024];  
            ssize_t ret=read(0,buf,sizeof(buf)-1);//系统调用从标准输入键盘获取字符串存入buf
   
            if(ret>0){  
                buf[ret-1]=0;    //将最后一个回车换行‘\n’刷新为0   
            }

            //将字符串分解为逐个命令存入指针数组中   
            char* argv[32];  
            int i=0;  
            argv[i]=buf;  
            char* start=buf;  
            while(*start)  
            {  
                if((*start)==' ')//若等于空格则一个命令结束   
                {  
      
                    *start=0;  
                    ++start;  
                    argv[++i]=start;  
                }  
                else  
                {  
                    ++start;  
                }  
            }  
            argv[i+1]=NULL;   //最后一个初始化为NULL   
              
            //实现3  
            //创建子进程调用exec()函数进行程序替换   
            pid_t id=fork();  
            if(id==0)  //child  
            {  
                //shell重定向>的实现   
                int j=0;  
                int fd;  
                for(j=0;j<=i;++j)  
                {  
                    if(strcmp(argv[j],">")==0)  //查找数组是否有重定向>符合   
                    {  
                        char* file=argv[j+1];     //有 ，下一个即为重定向的文件名  
                           
                        argv[j]=NULL;  //改变指针数组的有效大小 ，只执行j以前   
                          
                        close(1);    //关闭标准输出显示器 文件描述符1   
                        fd=open(file,O_WRONLY|O_CREAT|O_TRUNC,0666);  //则重新打开重定向的文件 其返回的文件描述符则为1   
                        //dup2(fd,STDOUT_FILENO);  //将fd文件描述符赋值于1   
                        break;  
                    }  
                }  
                //在以上重定向符实现中，即使文件描述符表中1的位置（文件描述符1）不在存储标准输出stdout文件，存储的为file文件地址  
                //又使指针数组有效大小只变为重定向符号>以前，即在下面程序替换中，执行argv中的命令不会显示在显示器上，而写在文件file中   
                //程序替换   
                execvp(argv[0],argv);     
                exit(1);  
            }  
            else  //father   //父进程只等待子进程结束   
            {  
                int status=0;  
                pid_t ret=waitpid(id,&status,0);  
            }  
        }  
    }  
    int main()  
    {  
        Myshell();  
        return 0;  
    }  
