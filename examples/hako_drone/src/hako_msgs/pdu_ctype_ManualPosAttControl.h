#ifndef _pdu_ctype_hako_msgs_ManualPosAttControl_H_
#define _pdu_ctype_hako_msgs_ManualPosAttControl_H_

#include "pdu_primitive_ctypes.h"
#include "geometry_msgs/pdu_ctype_Twist.h"
#include "geometry_msgs/pdu_ctype_Vector3.h"

typedef struct {
        Hako_bool do_operation;
        Hako_Twist    posatt;
} Hako_ManualPosAttControl;

#endif /* _pdu_ctype_hako_msgs_ManualPosAttControl_H_ */
