#ifndef _M_DOUBLE_BUFFER_H_
#define _M_DOUBLE_BUFFER_H_

class MDoubleBuffer
{
public:
    MDoubleBuffer(size_t max_len);
    ~MDoubleBuffer();
private:
    char *p_cur_buffer_;
    size_t cur_len_;
    char *p_bak_buffer_;
    size_t bak_len_;
    szie_t max_len_;
};

#endif
