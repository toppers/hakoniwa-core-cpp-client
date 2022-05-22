#include "hakoniwa_client.h"
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

using namespace hakoniwa;


static ErcdType init(char *ip_port)
{
  hakoniwa_core_init(ip_port);
  return Ercd_OK;
}

int main(int argc, char** argv) 
{
  char ip_port[128];
  char *param_file = nullptr;

  if (argc != 4) {
    printf("Usage: %s <ipaddr> <portno> {start|stop|reset|ls|status}\n", argv[0]);
    return 1;
  }
  sprintf(ip_port, "%s:%s", argv[1], argv[2]);
  ErcdType err = init(ip_port);
  if (err != Ercd_OK) {
    return 1;
  }
  const char *cmdp = (const char*)argv[3];
  if (strncmp(cmdp, "start", strlen("start")) == 0) {
    hakoniwa_core_start_simulation();
  }
  else if (strncmp(cmdp, "stop", strlen("stop")) == 0) {
    hakoniwa_core_stop_simulation();
  }
  else if (strncmp(cmdp, "reset", strlen("reset")) == 0) {
    hakoniwa_core_reset_simulation();
  }
  else if (strncmp(cmdp, "ls", strlen("ls")) == 0) {
    HakoniwaAssetInfoArrayType list;
    hakonwia_core_get_asset_list(&list);
    int i;
    for (i = 0; i < list.array_size; i++) {
      printf("asset[%d]=%s\n", i, list.entries[i].name);
    }
    hakonwia_core_free_asset_list(&list);
  }
  else if (strncmp(cmdp, "status", strlen("status")) == 0) {
    SimStatusType status = SimStatus_Terminated;
    const char *status_name[SimStatus_Num] = {
      "Stopped",
      "Runnable",
      "Running",
      "Stopping",
      "Terminated",
    };
    hakoniwa_core_get_simstatus(&status);
    printf("%s\n", status_name[status]);
  }
  else {
    printf("ERROR: invalid commad %s\n", cmdp);
  }
  return 0;
}