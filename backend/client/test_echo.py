from multiprocessing import Process
import os
import configparser
import sys

def get_threads_from_config():
    config = configparser.ConfigParser()
    config.read('/home/engage/github_projects/socket/backend/config/main.ini')
    return int(config['client']['threads'])


def run_program():
    os.system('python3 client_echo.py')


if __name__ == "__main__":
    sys.stdout = open('./log/echo.log', 'w')  # redirect print output to file

    processes = []
    N = get_threads_from_config()
    print("The number of threads: ", N)

    for _ in range(N):
        proc = Process(target=run_program)
        proc.start()
        processes.append(proc)

    for proc in processes:
        proc.join()
