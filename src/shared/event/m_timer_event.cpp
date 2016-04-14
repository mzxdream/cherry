#include <event/m_timer_event.h>
#include <event/m_event_loop.h>
#include <util/m_logger.h>

MTimerEvent::MTimerEvent()
    :event_active_(false)
    ,p_event_loop_(nullptr)
    ,start_time_(0)
    ,timeout_(0)
    ,repeat_count_(0)
    ,cb_(nullptr)
{
}

MTimerEvent::~MTimerEvent()
{
    DisableEvents();
}


