#ifndef _pdu_ctype_hako_msgs_HakoDroneCmdMove_H_
#define _pdu_ctype_hako_msgs_HakoDroneCmdMove_H_

#include "pdu_primitive_ctypes.h"
#include "hako_msgs/pdu_ctype_HakoDroneCmdHeader.h"

typedef struct {
        Hako_HakoDroneCmdHeader    header;
        Hako_float64 x;
        Hako_float64 y;
        Hako_float64 speed;
} Hako_HakoDroneCmdMove;

#endif /* _pdu_ctype_hako_msgs_HakoDroneCmdMove_H_ */
