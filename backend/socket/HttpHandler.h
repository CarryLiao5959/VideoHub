#include<string>
#include<unordered_map>
using namespace std;

class HttpHandler {
public:
    /**
     *  @brief HttpHandler内部状态
     */
    enum ERROR_TYPE {
        ERR_SUCCESS = 0,                // 无错误
        ERR_READ_REQUEST_FAIL,          // 读取请求数据失败
        ERR_NOT_IMPLEMENTED,            // 不支持一些特定的请求操作,例如 POST
        ERR_HTTP_VERSION_NOT_SUPPORTED, // 不支持当前客户端的http版本
        ERR_INTERNAL_SERVER_ERR,        // 程序内部错误
        ERR_CONNECTION_CLOSED,          // 远程连接已关闭
        ERR_BAD_REQUEST,                // 用户的请求包中存在错误,无法解析  
        ERR_SEND_RESPONSE_FAIL          // 响应包发送失败
    };
    // HttpHandler();
    /**
     * @brief   显式指定 client fd
     * @param   fd 连接的 fd, 初始值为 -1
     */
    explicit HttpHandler(int fd = -1);
    /**
     * @brief   释放所有 HttpHandler 所使用的资源
     * @note    注意,不会主动关闭 client_fd
     */
    ~HttpHandler();
    /**
     * @brief   为当前连接启动事件循环
     * @note    1. 在执行事件循环开始之前,一定要设置 client fd
     *          2. 异常处理不完备
     */
    void RunEventLoop();
    // 只有getFd,没有setFd,因为Fd必须在创造该实例时被设置
    int getClientFd() { return client_fd_; }
private:
    const int MAXBUF = 1024;
    int client_fd_;
    // http 请求包的所有数据
    string request_;
    // http 头部
    unordered_map<string, string> headers_; 
    
    // 请求方式
    string method_;
    // 请求路径
    string path_;
    // http版本号
    string http_version_;
    // 是否是 `持续连接`
    // NOTE: 为了防止bug的产生,对于每一个类中的isKeepAlive_来说,
    //       值只能从 true -> false,而不能再次从 false -> true
    bool isKeepAlive_;

    /**
     * @brief 当前解析读入数据的位置 
     * @note 该成员变量只在 
     *      readRequest -> parseURI -> parseHttpHeader -> RunEventLoop 
     * 内部中使用
     */
    size_t pos_;
    
    /**
     * @brief 将当前client_fd_对应的连接信息,以 LOG(INFO) 的形式输出
     */
    void printConnectionStatus();

    /**
     * @brief 从client_fd_中读取数据至 request_中
     * @return 0 表示读取成功, 其他则表示读取过程存在错误
     * @note 内部函数recvn在错误时会产生 errno
     */
    ERROR_TYPE readRequest();

    /**
     * @brief 从0位置处解析 请求方式\URI\HTTP版本等
     * @return 0 表示成功解析, 其他则表示解析过程存在错误
     */
    ERROR_TYPE parseURI();

    /**
     * @brief 从request_中的pos位置开始解析 http header
     * @return 0 表示成功解析, 其他则表示解析过程存在错误
     */
    ERROR_TYPE parseHttpHeader();
    
    /**
     * @brief   发送响应报文给客户端
     * @param   responseCode        http 状态码, http报文第二个字段
     * @param   responseMsg         http 报文第三个字段
     * @param   responseBodyType    返回的body类型,即 Content-type
     * @param   responseBody        返回的body内容
     * @return 0 表示成功发送, 其他则表示发送过程存在错误
     */
    ERROR_TYPE sendResponse(const string& responseCode, const string& responseMsg, 
                      const string& responseBodyType, const string& responseBody);
    
    /**
     * @brief 发送错误信息至客户端
     * @param errCode   错误http状态码
     * @param errMsg    错误信息, http报文第三个字段
     * @return 0 表示成功发送, 其他则表示发送过程存在错误
     */
    ERROR_TYPE handleError(const string& errCode, const string& errMsg);

    /**
     * @brief 将传入的字符串转义成终端可以直接显示的输出
     * @param str 待输出的字符串
     * @return 转义后的字符串
     * @note  是将 '\r' 等无法在终端上显示的字符,转义成 "\r"字符串 输出
     */
    string escapeStr (const string& str);
};