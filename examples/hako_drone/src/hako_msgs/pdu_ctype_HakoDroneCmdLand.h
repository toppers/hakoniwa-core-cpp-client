#ifndef _pdu_ctype_hako_msgs_HakoDroneCmdLand_H_
#define _pdu_ctype_hako_msgs_HakoDroneCmdLand_H_

#include "pdu_primitive_ctypes.h"
#include "hako_msgs/pdu_ctype_HakoDroneCmdHeader.h"

typedef struct {
        Hako_HakoDroneCmdHeader    header;
        Hako_float64 height;
        Hako_float64 speed;
} Hako_HakoDroneCmdLand;

#endif /* _pdu_ctype_hako_msgs_HakoDroneCmdLand_H_ */
