#include <util/m_circle_buffer.h>
#include <cstring>

MCircleBuffer::MCircleBuffer(size_t len)
    :len_(len+1)
{
    p_buf_ = new char[len_];
    p_start_ = p_buf_;
    p_end_ = p_buf_;
}

MCircleBuffer::~MCircleBuffer()
{
    delete p_buf_;
}

bool MCircleBuffer::Peek(void *p_buf, size_t len)
{
    if (p_end_ >= p_start_)
    {
        if (len > p_end_ - p_start_)
        {
            return false;
        }
        memcpy(p_buf, p_start_, len);
        p_start_ += len;
        return true;
    }
    else
    {
        if (len <= (len_ - (p_start_ - p_buf_)))
        {
            memcpy(p_buf, p_start_, len);
            p_start_ += len;
            if (p_start_ - p_buf_ >= len_)
            {
                p_start_ -= len_;
            }
            return true;
        }
        else if (len <= (len_ - (p_start_ - p_end_)))
        {
            size_t end_len = len_ - (p_start_ - p_buf_);
            memcpy(p_buf, p_start_, end_len);
            p_start_ -= (len_ - len);
            memcpy(p_buf + end_len, p_buf_, p_start_ - p_buf_);
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool MCircleBuffer::Append(const char *p_buf, size_t len)
{
    if (p_end_ >= p_start_)
    {
        if (len >= len_ - (p_end_ - p_start_))
        {
            return false;
        }
        if (len <= len_ - (p_end_ - p_buf_))
        {
            memcpy(p_end_, p_buf, len);
            p_end_ += len;
            if (p_end_ - p_buf_ >= len)
            {
                p_end_ -= len_;
            }
            return true;
        }
        else
        {
            size_t end_len = len_ - (p_end_ - p_buf_);
            memcpy(p_end_, p_buf, end_len);
            p_end_ -= (len_ - len);
            memcpy(p_buf_, p_buf + end_len, p_end_ - p_buf_);
            return true;
        }
    }
    else
    {
        if (len >= p_start_ - p_end_)
        {
            return false;
        }
        memcpy(p_end_, p_buf, len);
        p_end_ += len;
        return true;
    }
}
