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
       while(msgs.size()>0) {
           // 每次取队列头，然后处理完再把头扔了
           FileMsg msg=msgs.front();
           msgs.erase(msgs.begin());
           this->currentMsg=msg;     //设置当前项目
           this->currentMsg.status=0;
           id=msg.id;
           if(this->msgs.size()>0)
               nextId=this->msgs.front().id;
           else nextId = -1;
           string path = msg.filepath;
           SOCKET  sock=login(this->Username,this->Password,this->Ip);
           cwd(sock,this->remote_path);
           if(msg.UpOrDown==0)       //上传
           {
               upload(sock, path, id);
           }
           else  if(msg.UpOrDown==1){
               download(path,id);
           }
           else if(msg.UpOrDown==2)    //上传项目的断点续传
           {
             ;
           }
           else if(msg.UpOrDown==3)    //下载项目的断点续传
           {
               cout<<"断点续传"<<endl;
               downloadContinue();
           }
           if(this->currentMsg.status==0)          //没有被暂停的任务就会发送完成，被中断的项目就不会发送
               emit(finishOne(id, nextId));        //任务为一个文件则发送一个任务完成
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
        // 文件和目录的暂停，需要在这里做的一样
        // 不同的部分的数据在下载的函数内更新了
            this->currentMsg.status = 1;  //修改状态,会在currentMsg的一定是下载中的任务
            this->currentMsg.UpOrDown = 3;
            this->stopedMsgs.push_back(this->currentMsg);
    }
    else {
       //暂停
        for(int i=0;i<this->msgs.size();i++)
        {
            if(this->msgs[i].id==id)    //如果是非正在下载进程
            {
                // 文件和目录的暂停，需要在这里做的一样
                // 不同的部分在下载的函数内更新了，
                    cout << "按钮点击事件触发" << endl;
                    this->msgs[i].status = 1;
                    this->msgs[i].UpOrDown = 3;       //切换成下载的断点续传任务
                    this->stopedMsgs.push_back(this->msgs[i]);
                    this->msgs.erase(this->msgs.begin()+i);   //项目擦除
                    cout.flush();
                    return;
            }
        }
        //恢复
        for(int i=0;i<this->stopedMsgs.size();i++)
        {
            int finish=0;
            if(this->stopedMsgs[i].id==id)
            {
                   // 文件和文件夹的处理无区别，有区别的部分都是实时更新到Msg了
                    this->stopedMsgs[i].status=0; //恢复下载
                    this->msgs.push_back(this->stopedMsgs[i]);
                    finish=1;

            }
            if(finish==1&&this->stopedMsgs[i].id==id)
            {
                //从被暂停列表中移除项目
                this->stopedMsgs.erase(this->stopedMsgs.begin()+i);
            }
        }
    }
}


