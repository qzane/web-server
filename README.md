Title         : web-server
Sub Title     : an event-driven web server and client
Author        : qzane
Email         : qzane@live.com

[TITLE]

# Content

[TOC]

# What is this

[This web-server project] contains the following files:

* server.c
* server_loop.c
* server_multi_process.c
* client.c
* client_multi_process.c

[This web-server project]: http://github.com/qzane/web-server
## Install

```
   gcc -o client client.c
   gcc -o client_multi_process client_multi_process.c
   gcc -o server server.c
   gcc -o server_loop server_loop.c
   gcc -o server_multi_process server_multi_process.c
```

## server & client

on server machine

```
  >>>./server <port>
```
on client machine

```
  >>>./client <server's ip> <port>
  Please enter the message: num=10-
  result=29
```
__explain:<br>__
The server listen to the specified prot.<br>
Once it receive a message like "num=x-",<br>
it will reply  "result=y",<br>
and the value of y equals  $x*3-1$.<br>
Also, it will wait for about 0.32s for each request simulating the IO delay.
<br><br>
The client connect to the server with its ip address and port.<br>
Once the connection is set, the client'll ask you to input a message.<br>
Then, the client will sent the message to the server and display its reply.

## server_loop

```
  >>>./server_loop <port>
```

Very similar to __server__ except that it will wait for another connection after each work.

## client_multi_process

```
  >>>./client_multi_process <server's ip> <port>
```

The main process of this program generate 20 sub-process,<br>
each of which will send a message like "num=count-" to the server.<br>
the values of count are 0,1,2...19.
 

## server_multi_process

```
  >>>./server_multi_process <port>
```
The real event-driven web server.<br>
The  behavior is similar to __server_loop__,<br>
but once this program get a new request,<br>
it will generate a new process to deal with that request,<br>
so that the main process can keep on listening.

# Result

## test
run the following commands on server machine:
``` python
  >>>./server_loop <port>          # test 1
  >>>./server_multi_process <port> # test 2
```
for each test, run this commands on client machine:

```
  >>>time ./client_multi_process <server's ip> <port>
```

## result
__test 1:__
```
real    0m5.151s
user    0m0.001s
sys     0m0.002s
```
__test 2:__
```
real    0m0.363s
user    0m0.000s
sys     0m0.002s
```

__PS:__ the client detects some errors when using server_loop <br>
but this thing didn't happened when using server_multi_process.<br>
So, I guess that the $backlog$ argument of $listen(2)$ only limit
programs with single thread.


# design

## server & client & server_loop
I learnt sockets programming from the web-site:
>http://www.linuxhowtos.org/C_C++/socket.htm

## client_multi_process

``` cpp
    for(count=0;count<PROCESS_NUM && fork()!=0;++count)
        ;//make PROCESS_NUM copies

    if(count==PROCESS_NUM){//root process
        while(wait(NULL)!=-1)
            ;
        printf("\nAll finished!\n");
        exit(0);
    }
```

## server_multi_process

``` cpp
    newsockfd = accept(sockfd,(struct sockaddr *) 
                       &cli_addr,&clilen);
    if(fork()==0){
        process(newsockfd);       
        close(newsockfd);
        printf("Work%4d done!\n",count);
        exit(0);
    }
    close(newsockfd);
    ++count;
```