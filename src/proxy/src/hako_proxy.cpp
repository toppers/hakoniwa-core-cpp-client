#include "proc/hako_process.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
using json = nlohmann::json;

using namespace hako::client;
using namespace hako::client::proxy;


int main(int argc, char** argv)
{
    if ((argc != 2)) {
        printf("Usage: %s <param_file>\n", argv[0]);
        return 1;
    }

    return 0;
}
