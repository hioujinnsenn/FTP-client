//
// Created by asus on 2020/3/20.
//

#include "upload_qThread.h"
#include <QDebug>
#include "ftpsock/upload.h"

uploadThread::uploadThread(string username,string password,string ip) //构造器
{
    // 只传递必要的信息，文件信息通过信号槽传递
    this->Username=(char*)malloc(400);
    this->Password=(char*)malloc(400);
    this->Ip=(char*)malloc(400);
    memset(this->Username,0,400);
    memset(this->Password,0,400);
    memset(this->Ip,0,400);
    sprintf(this->Username,username.data());
    sprintf(this->Password,password.data());
    sprintf(this->Ip,ip.data());

}

uploadThread::~uploadThread()   //析构器
{
    qDebug()<<"uploadThread::~uploadThread";
}

void uploadThread::setStop()
{
    if(state==0){
        state=1;    //进度暂停
    }
    else if(state==1){
        state=0;    //进度继续（开始）
    }
}
void uploadThread::receive_filemsg(FileMsg msg)   //  接受UI界面传递过来的文件信息
{

    this->msgs.push_back(msg);         //更改为deque类型，因为queue不好使用下标访问
}
void uploadThread::run()             // 此进程修改成和UI主界面共生存在
{
   while(thread_alive)               // 应该设置成无限循环，不停止,通过设置thread_alive实现终止线程
   {
       if(this->msgs.size()>0){
           // 每次取队列头，然后处理完再把头扔了
           FileMsg msg=msgs[0];
           msgs.erase(msgs.begin());   //先访问第一个再移除
           if(msg.status.testAndSetOrdered(1,1))       //属于被暂停的项目，应该扔回队尾继续等待
           {
               this->msgs.push_back(msg);
               continue;
           }
           this->currentMsg=msg;     //设置当前项目
           id=msg.id;
           if (this->msgs.size()>0)
               nextId = msgs.front().id;
           else nextId = -1;
           string path = msg.filepath;
           SOCKET  sock=login(this->Username,this->Password,this->Ip);
           cwd(sock,this->remote_path);
           if(msg.UpOrDown==0)       //上传
           {
               upload(sock, path, id);
           }
           else  if(msg.UpOrDown==1){
               download(sock,path,id);
           }
           else if(msg.UpOrDown==2)
           {
             ;
           }
           else if(msg.UpOrDown==3)    //下载项目的断点续传
           {
               cout<<"断点续传"<<endl;
              downloadContinue(sock);
           }
           if(this->currentMsg.status==0)          //没有被暂停的任务就会发送完成，被中断的项目就不会发送
               emit(finishOne(id, nextId));        //任务为一个文件则发送一个任务完成
           SendCommand(sock,QUIT);
           this->currentMsg=FileMsg();             //每次都要新建一个空的，全0的数据覆盖掉旧的，保证receive_pause_id是正确的
           this->currentMsg.id=-1;                 //标记为-1的都是没有用的空项目
       }
   }
}
void uploadThread::receive_remote_path(string path) {
    this->remote_path=path;
}



void  uploadThread::receive_local_path(string path)
{
    this->local_path=path;
}

// 槽，接收暂停任务的函数，
// 下载和上传的暂停都是设置
//  恢复也是用同一个事件即可
// 通过模二的加法即可
void uploadThread::receive_pause_id(int id){

    // 暂停的时候，是不会有currentMsg的，然后旧的信息没有清除，一直卡在这
    if(this->currentMsg.id==id) {
        this->currentMsg.status.fetchAndStoreOrdered(1)  ;//修改状态,会在currentMsg的一定是下载中的任务
        cout<<"暂停下载"<<this->currentMsg.status.fetchAndAddOrdered(0)<<endl;
    }
    else {
        for(int i=0;i<this->msgs.size();i++)
        {
            if(this->msgs[i].id==id)    //如果是非正在下载进程
            {
                cout<<"按钮点击事件触发"<<endl;
                if(this->msgs[i].status.testAndSetOrdered(0,1)) {
                    while(this->msgs[i].status.testAndSetOrdered(0,1))
                        ;
                    cout<<"暂停下载";
                    cout<<this->msgs[i].status.fetchAndAddOrdered(0)<<endl;
                    cout.flush();
                }
                else if (this->msgs[i].status.testAndSetOrdered(1,0))
                {
                    while(this->msgs[i].status.testAndSetOrdered(1,0))
                        ;
                    cout<<"恢复下载";
                    cout<<this->msgs[i].status.fetchAndAddOrdered(0)<<endl;
                    cout.flush();
                }

                cout.flush();
                break;
            }
        }
    }
}


