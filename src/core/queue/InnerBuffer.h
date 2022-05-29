#ifndef _INNER_BUFFER_H
#define _INNER_BUFFER_H

#include <cstdint>
#include <queue>
#include <string>
#include <vector>

#include "../../networking/frame/IFrameBody.h"
#include "../../utils/config/ConfigSetting.h"
#include "../../utils/log/Log.h"

namespace faker_tsn {

class InnerBuffer {
  private:
    std::string m_deviceName;         /* device names */
    const uint32_t m_capacity;        /* capacity of buffer */
    unsigned short m_portIndex;       /* port index */
    uint8_t m_pcp;                    /* priority code */
    uint32_t m_residualCapacity;      /* residual capacity of buffer */
    uint32_t m_usedCapacity;          /* used capacity of buffer */
    std::deque<IFrameBody*> m_buffer; /* frame body container */

  public:
    InnerBuffer(unsigned short portIndex, uint8_t pcp, uint32_t capacity);

    ~InnerBuffer();

    void enqueue(IFrameBody* frameBody);

    /* dequeue head frame */
    IFrameBody* dequeue();

    /* dequeue frame by index */
    IFrameBody* dequeue(uint32_t index);

    /* get frame by index */
    IFrameBody* getBy(uint32_t index);

    /* remove frame by index */
    void removeBy(uint32_t index);

    /* drop frame */
    void drop(IFrameBody* frameBody);

    inline uint8_t getPCP() {
        return this->m_pcp;
    }

    inline bool isEmpty() {
        return this->m_buffer.empty();
    }

    inline unsigned int getCapacity() {
        return this->m_capacity;
    }

    inline unsigned int getResidualCapacity() {
        return this->m_residualCapacity;
    }

    inline unsigned int getUsedCapacity() {
        return this->m_usedCapacity;
    }
};

}  // namespace faker_tsn

#endif