#include "hako_asset.h"
#include "hako_conductor.h"
#include <stdio.h>
#include <stdlib.h>
#include "hako_msgs/pdu_ctype_HakoDroneCmdTakeoff.h"
#include "hako_msgs/pdu_ctype_HakoDroneCmdMove.h"
#include "hako_msgs/pdu_ctype_HakoDroneCmdLand.h"
#define HAKO_AVATOR_CHANNLE_ID_MOTOR        0
#define HAKO_AVATOR_CHANNLE_ID_POS          1
#define HAKO_AVATOR_CHANNLE_ID_COLLISION    2
#define HAKO_AVATOR_CHANNLE_ID_MANUAL       3
#define HAKO_AVATOR_CHANNEL_ID_DISTURB      4
#define HAKO_AVATOR_CHANNEL_ID_CMD_TAKEOFF  5
#define HAKO_AVATOR_CHANNEL_ID_CMD_MOVE     6
#define HAKO_AVATOR_CHANNEL_ID_CMD_LAND     7


#ifdef _WIN32
static inline void usleep(long microseconds) {
    Sleep(microseconds / 1000);
}
#else
#include <unistd.h>
#endif

static void do_cmd_takeoff(void)
{
    printf("INFO: start takeoff\n");
    int result = 0;
    Hako_HakoDroneCmdTakeoff cmd_takeoff;
    cmd_takeoff.header.request = 1;
    cmd_takeoff.header.result = 0;
    cmd_takeoff.header.result_code = 0;
    cmd_takeoff.height = 5;
    cmd_takeoff.speed = 5;
    hako_asset_pdu_write("DroneFlightController", HAKO_AVATOR_CHANNEL_ID_CMD_TAKEOFF, (const char*)&cmd_takeoff, sizeof(cmd_takeoff));
    while (result == 0) {
        if (hako_asset_pdu_read("DroneFlightController", HAKO_AVATOR_CHANNEL_ID_CMD_TAKEOFF, (char*)&cmd_takeoff, sizeof(cmd_takeoff)) == 0) {
            if (cmd_takeoff.header.result) {
                cmd_takeoff.header.result = 0;
                hako_asset_pdu_write("DroneFlightController", HAKO_AVATOR_CHANNEL_ID_CMD_TAKEOFF, (const char*)&cmd_takeoff, sizeof(cmd_takeoff));
                printf("DONE\n");
                break;
            }
        }
        usleep(1000*100);
    }
}
static void do_cmd_move(double x, double y)
{
    printf("INFO: start move\n");
    int result = 0;
    Hako_HakoDroneCmdMove cmd;
    cmd.header.request = 1;
    cmd.header.result = 0;
    cmd.header.result_code = 0;
    cmd.x = x;
    cmd.y = y;
    cmd.speed = 5;
    hako_asset_pdu_write("DroneFlightController", HAKO_AVATOR_CHANNEL_ID_CMD_MOVE, (const char*)&cmd, sizeof(cmd));
    while (result == 0) {
        if (hako_asset_pdu_read("DroneFlightController", HAKO_AVATOR_CHANNEL_ID_CMD_MOVE, (char*)&cmd, sizeof(cmd)) == 0) {
            if (cmd.header.result) {
                cmd.header.result = 0;
                hako_asset_pdu_write("DroneFlightController", HAKO_AVATOR_CHANNEL_ID_CMD_MOVE, (const char*)&cmd, sizeof(cmd));
                printf("DONE\n");
                break;
            }
        }
        usleep(1000*100);
    }
}
static void do_cmd_land(void)
{
    printf("INFO: start land\n");
    int result = 0;
    Hako_HakoDroneCmdLand cmd;
    cmd.header.request = 1;
    cmd.header.result = 0;
    cmd.header.result_code = 0;
    cmd.height = 0;
    cmd.speed = 5;
    hako_asset_pdu_write("DroneFlightController", HAKO_AVATOR_CHANNEL_ID_CMD_LAND, (const char*)&cmd, sizeof(cmd));
    while (result == 0) {
        if (hako_asset_pdu_read("DroneFlightController", HAKO_AVATOR_CHANNEL_ID_CMD_LAND, (char*)&cmd, sizeof(cmd)) == 0) {
            if (cmd.header.result) {
                cmd.header.result = 0;
                hako_asset_pdu_write("DroneFlightController", HAKO_AVATOR_CHANNEL_ID_CMD_LAND, (const char*)&cmd, sizeof(cmd));
                printf("DONE\n");
                break;
            }
        }
        usleep(1000*100);
    }
}
int main(int argc, const char* argv[])
{
    int ret = hako_initialize_for_external();
    if (ret != 0) {
        printf("ERORR: hako_initialize_for_external() returns %d.", ret);
        return 1;
    }
    do_cmd_takeoff();
    do_cmd_move(0, 5);
    //do_cmd_move(-5, -5);
    //do_cmd_move(0, 0);
    do_cmd_land();
    return 0;
}
