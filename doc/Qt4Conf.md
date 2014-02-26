用*apt-get*安装好Qt4的环境后，执行以下操作，完成Qt4在本项目中的配置

由于Codeblocks需要Qt4装在一个文件夹内，既一个文件夹内有Qt4的include，lib等目录，而Ubuntu下apt安装的结果会让Qt的各个文件分散在各处，所以需要配置一下。

流程：新建一个总目录，给Qt4每个部分建立一个到总目录的软连接

以管理员身份(sudo) 执行命令：
    
    mkdir /usr/myqt4
    ln -s /usr/lib/x86_64-linux-gnu/ /usr/myqt4/lib
    ln -s /usr/include/qt4/ /usr/myqt4/include
    ln -s /usr/share/qt4/ /usr/myqt4/share