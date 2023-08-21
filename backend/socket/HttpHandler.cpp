#include "HttpHandler.h"

HttpHandler::HttpHandler(int fd = -1){
    client_fd_=fd;
}

HttpHandler::~HttpHandler(){}

HttpHandler::ERROR_TYPE HttpHandler::readRequest()
{
    // 清除之前的数据
    request_.clear();
    pos_ = 0;

    char buffer[MAXBUF];
    
    // 循环阻塞读取 ------------------------------------------
    for(;;)
    {
        ssize_t len = readn(client_fd_, buffer, MAXBUF, true, true);
        if(len < 0)
            return ERR_READ_REQUEST_FAIL;
        /** 
         * 如果此时没读取到信息并且之前已经读取过信息了,则直接返回.
         * 这里需要注意,有些连接可能会提前连接过来,但是不会马上发送数据.因此需要阻塞等待
         * 这里有个坑点: chromium在每次刷新过后,会额外开一个连接,用来缩短下次发送请求的时间
         * 也就是说这里大概率会出现空连接,即连接到了,但是不会马上发送数据,而是等下一次的请求.
         * 
         * 如果读取到的字节数为0,则说明远程连接已经被关闭.
         */
        else if(len == 0)
        {
            // 对于已经读取完所有数据的这种情况
            if(request_.length() > 0)
                // 直接停止读取
                break;
            // 如果此时既没读取到数据,之前的 request_也为空,则表示远程连接已经被关闭
            else
                return ERR_CONNECTION_CLOSED;
        }
        // 将读取到的数据组装起来
        string request(buffer, buffer + len);
        request_ += request;

        // 由于当前的读取方式为阻塞读取,因此如果读取到的数据已经全部读取完成,则直接返回
        if(static_cast<size_t>(len) < MAXBUF)
            break;
    }
    return ERR_SUCCESS;
}