#ifndef _pdu_ctype_hako_msgs_Collision_H_
#define _pdu_ctype_hako_msgs_Collision_H_

#include "pdu_primitive_ctypes.h"
#include "geometry_msgs/pdu_ctype_Point.h"
#include "geometry_msgs/pdu_ctype_Vector3.h"

typedef struct {
        Hako_bool collision;
        Hako_uint32 contact_num;
        Hako_Vector3    relative_velocity;
        Hako_Point contact_position[10];
        Hako_float64 restitution_coefficient;
} Hako_Collision;

#endif /* _pdu_ctype_hako_msgs_Collision_H_ */
