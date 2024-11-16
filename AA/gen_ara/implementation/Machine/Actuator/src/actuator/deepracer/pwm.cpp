#include "deepracer/servo/pwm.h"

namespace PWM {
    /// Max size of the character buffer used to concat the file paths.
    #define MAX_BUF 64
    // The following pwm dev id and base path were given by pegatron.
    #define PWMDEV "0000:00:17.0"
    #define BASE_SYS_PATH "/sys/class/pwm/"

    /// Helper method that writes the given value into a specified path.
    /// @param path Path to the file where the value is to be written.
    /// @param value Value to write in the file given by path.
  	void writePWM(const char *path, int value) {
        int fd, len;
        fd = open(path, O_WRONLY);
        if (fd < 0) {
            return;
        }
        char buf[MAX_BUF];
        len = snprintf(buf, sizeof(buf), "%d", value);
        write(fd, buf, len);
        close(fd);
    }
    /// Rewriting the command to dynamically find the pwmchip%d directory and return the syspath
    /// `ls -al /sys/class/pwm/ | grep "0000:00:17.0" | awk '{ print $9}'`
    /// @returns Syspath pointing to the pwmchip directory.
    const std::string getSysPath(){
        auto syspath = std::string(BASE_SYS_PATH) + "pwmchip0";
        DIR *dir = opendir(BASE_SYS_PATH);
        if (!dir) {
            return syspath;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string filepath = std::string(BASE_SYS_PATH) + entry->d_name;

            struct stat path_stat;
            if (lstat(filepath.c_str(), &path_stat) == 0 && S_ISLNK(path_stat.st_mode)) {
                char symlinkTarget[MAX_BUF];
                ssize_t len = readlink(filepath.c_str(), symlinkTarget, sizeof(symlinkTarget) - 1);
                if (len != -1) {
                    symlinkTarget[len] = '\0';
                    if (strstr(symlinkTarget, PWMDEV)) {
                        syspath = std::string(BASE_SYS_PATH) + entry->d_name;
                        break;
                    }
                }
            }
        }
        closedir(dir);
        return syspath;
    }

    Servo::Servo(int channel)
      : channel_(channel),
        period_(0),
        duty_(0),
        syspath_(getSysPath())
    {
        char exportPath[MAX_BUF];
        snprintf(exportPath, sizeof(exportPath), (syspath_ + std::string("/pwm%d")).c_str(), channel_);
        struct stat st;
        if (stat(exportPath, &st) == 0) {
            return;
        }
    
        writePWM((syspath_ + std::string("/export")).c_str(), channel_);
    }

    /// Setter for the PWM period.
    /// @param period Desired period in ms.
    void Servo::setPeriod(int period) {
        char periodPath[MAX_BUF];
        snprintf(periodPath, sizeof(periodPath), (syspath_ + std::string("/pwm%d/period")).c_str(), channel_);
        writePWM(periodPath, period);
    }

    /// Setter for the duty cycle, this is what determines how much the servo actuates.
    /// @param Desired duty cycle.
    void Servo::setDuty(int duty) {
        char dutyPath[MAX_BUF];
        snprintf(dutyPath, sizeof(dutyPath), (syspath_ + std::string("/pwm%d/duty_cycle")).c_str(), channel_);
        writePWM(dutyPath, duty);
    }

    /// @returns Current value of the period.
    int Servo::getPeriod() const {
        return period_;
    }

    /// @returns Current value of the duty cycle.
    int Servo::getDuty() const {
        return duty_;
    }
}
