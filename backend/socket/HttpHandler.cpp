#include "HttpHandler.h"

// HttpHandler::HttpHandler(int fd = -1){
//     m_client.setfd(fd);
// }

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
        ssize_t len = m_client.readn(buffer, MAXBUF, true);
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
        // if(static_cast<size_t>(len) < MAXBUF)
        //     break;
    }
    return ERR_SUCCESS;
}

HttpHandler::ERROR_TYPE HttpHandler::parseURI()
{
    // if(request_.empty())   return ERR_BAD_REQUEST;

    // size_t pos1, pos2;
    
    // pos1 = request_.find("\r\n");
    // if(pos1 == string::npos)    return ERR_BAD_REQUEST;
    // string&& first_line = request_.substr(0, pos1);
    // // a. 查找get
    // pos1 = first_line.find(' ');
    // if(pos1 == string::npos)    return ERR_BAD_REQUEST;
    // method_ = first_line.substr(0, pos1);

    // string output_method = "Method: ";
    // if(method_ == "GET")
    //     output_method += "GET";
    // else
    //     return ERR_NOT_IMPLEMENTED;
    // LOG(INFO) << output_method << endl;

    // // b. 查找目标路径
    // pos1++;
    // pos2 = first_line.find(' ', pos1);
    // if(pos2 == string::npos)    return ERR_BAD_REQUEST;

    // // 获取path时,注意去除 path 中的第一个斜杠
    // pos1++;
    // path_ = first_line.substr(pos1, pos2 - pos1);
    // // 如果 path 为空,则添加一个 . 表示当前文件夹
    // if(path_.length() == 0)
    //     path_ += ".";
    
    // // 判断目标路径是否是文件夹
    // struct stat st;
    // if(stat(path_.c_str(), &st) == 0)
    // {
    //     // 如果试图打开一个文件夹,则添加 index.html
    //     if (S_ISDIR(st.st_mode))
    //         path_ += "/index.html";
    // }

    // LOG(INFO) << "Path: " << path_ << endl;

    // // c. 查看HTTP版本
    // // NOTE 这里只支持 HTTP/1.0 和 HTTP/1.1
    // pos2++;
    // http_version_ = first_line.substr(pos2, first_line.length() - pos2);
    // LOG(INFO) << "HTTP Version: " << http_version_ << endl;

    // // 检测是否支持客户端 http 版本
    // if(http_version_ != "HTTP/1.0" && http_version_ != "HTTP/1.1")
    //     return ERR_HTTP_VERSION_NOT_SUPPORTED;
    // // 设置只在 HTTP/1.1时 允许 持续连接
    // if(http_version_ != "HTTP/1.1")
    //     isKeepAlive_ = false;

    // // 更新pos_
    // pos_ = first_line.length() + 2;
    return ERR_SUCCESS;
}

void HttpHandler::RunEventLoop(){}