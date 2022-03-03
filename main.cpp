#include "include/threads.h"

int main(int argc, char **argv) {
    int number_of_threads = 0;
    thread_data *td = NULL;
    enum order_of_finishing_threads direction;

    if (argc == 2 && ((strcmp(*(argv + 1), "--help") == 0) || (strcmp(*(argv + 1), "-h") == 0))) {
        display_help();
        return ERROR;
    } else if (argc == 1 || argc > 3) {
        std::cout << "Incorrect number of arguments, see '--help or -h'" << std::endl;
        return ERROR;
    } else if (atoi(*(argv + 1)) < 3 || atoi(*(argv + 1)) > 100 ||
               (strcmp(*(argv + 2), "inc") != 0 && strcmp(*(argv + 2), "dec") != 0)) {
        std::cout << "Incorrect arguments, see '--help or -h'" << std::endl;
        return ERROR;
    } else {
        if ((strcmp(*(argv + 2), "inc") == 0)) {
            direction = inc;
        } else if ((strcmp(*(argv + 2), "dec") == 0)) {
            direction = dec;
        }
        number_of_threads = atoi(*(argv + 1));
    }

    if (createThreads(&td, number_of_threads) == SUCCESS) {
        finishThreads(td, number_of_threads, direction);
    } else {
        std::cout << "Error occured." << std::endl;
    }

    exit(EXIT_SUCCESS);
}
