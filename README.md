启动mysql:docker start MySQl8.0
启动redis:进入/usr/local/nginx/sbin目录，执行./nginx -c conf/chatserver.conf(以指定配置文件启动nginx)

在根目录下创建build和bin文件夹，然后执行./autobuild.sh  一键编译项目  编译后在bin目录下生成ChatServer和ChatClient两个可执行文件

打开四个终端，均进入/home/czl/Documents/Code/chatserver/bin目录下，
终端1和终端2执行./ChatServer 127.0.0.1 6000和./ChatServer 127.0.0.1 6002，启动服务器
终端3和终端4均执行 ./ChatClient 127.0.0.1 8000连接nginx进行反向代理连接到服务器(这是启动了客户端)


