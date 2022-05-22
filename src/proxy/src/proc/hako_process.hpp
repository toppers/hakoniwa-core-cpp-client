#ifndef _HAKO_PROCESS_HPP_
#define _HAKO_PROCESS_HPP_

#include <string>
#include <memory>
#include <thread>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

namespace hako::client::proxy {

class ProcessManager {
public:
    ProcessManager() {}
    ~ProcessManager() {
    }
    bool invoke() {
        if (this->is_running()) {
            printf("ERROR: already running...\n");
            return false;
        }
        if (this->current_dir_ == nullptr) {
            return false;
        }
        if (this->binary_path_ == nullptr) {
            return false;
        }
        int ret = fork();
        if (ret < 0) {
            return false;
        }
        else if (ret == 0) {
            //another process context start
            ret = ProcessManager::monitoring(this);
            exit(ret);
        }
        this->status_is_running = true;
        this->pid_ = ret;
        printf("pid:%d\n", ret);
        return true;
    }
    void terminate()
    {
        int status;
        int ret = kill(this->pid_, SIGTERM);
        printf("kill ret=%d\n", ret);
        ret = wait(&status);
        sync();
        sync();
        sync();
        sync();
        sync();
        this->finish();
        printf("wait ret=%d status=0x%x\n", ret, status);
    }
    bool is_running()
    {
        return this->status_is_running;
    }

    bool set_current_dir(std::string dir) {
        struct stat buf;
        int ret = stat(dir.c_str(), &buf);
        if (ret < 0) {
            printf("ERROR: not found %s\n", dir.c_str());
            return false;
        }
        this->current_dir_.reset();
        this->current_dir_ = std::make_unique<std::string>(dir);
        return true;
    }
    bool set_binary_path(std::string path) {
        this->binary_path_.reset();
        this->binary_path_ = std::make_unique<std::string>(path);
        this->the_args.clear();
        this->add_option(path);
        return true;
    }
    bool add_option(std::string option) {
        if (this->binary_path_) {
            the_args.push_back(option);
            printf("add_option:%s\n", option.c_str());
            return true;
        }
        else {
            return false;
        }
    }

private:
    std::unique_ptr<std::string> binary_path_ = nullptr;
    std::unique_ptr<std::string> current_dir_ = nullptr;
    bool status_is_running = false;
    std::vector<std::string> the_args;
    int pid_;
    void finish()
    {
        this->status_is_running = false;
    }
    static int monitoring(ProcessManager *mgrp)
    {
        printf("monitring:\n");
        int exec_arg_count = 0;
        char **exec_args = (char**)malloc( (mgrp->the_args.size() + 1) * sizeof(char*));
        if (exec_args == NULL) {
            return -1;
        }
        int ret = chdir(mgrp->current_dir_->c_str());
        if (ret >= 0) {
            printf("exec_args_count=%ld\n", mgrp->the_args.size());
            std::size_t i;
            for (i = 0; i < mgrp->the_args.size(); i++) {
                printf("the_args[%lu]=%s\n", i, mgrp->the_args.at(i).c_str());
                exec_args[exec_arg_count++] = (char*)mgrp->the_args.at(i).c_str();
            }
            exec_args[exec_arg_count] = NULL;
            ret = execv(mgrp->binary_path_->c_str(), exec_args);
        }
        free(exec_args);
        return ret;
    }

};

}

#endif /* _HAKO_PROCESS_HPP_ */
