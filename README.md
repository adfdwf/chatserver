启动mysql:docker start MySQl8.0
启动redis:进入/usr/local/nginx/sbin目录，执行./nginx -c conf/chatserver.conf(以指定配置文件启动redis)

打开四个中断，均进入/home/czl/Documents/Code/chatserver/bin目录下，
终端1和终端2执行./ChatServer 127.0.0.1 6000和./ChatServer 127.0.0.1 6002，启动服务器
终端3和终端4均执行 ./ChatClient 127.0.0.1 8000连接nginx进行反向代理连接到服务器(这是启动了客户端)


