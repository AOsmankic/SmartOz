import time
import logging
import sys
from ipc.ipc import InternetRequest
from client import OzInnerClient
logger = logging.getLogger()

def run():
    oz_server = OzInnerClient()
    while True:
        # cur_time = get_time()
        # oz_server.send_message(cur_time)
        internet_request = generate_internet_request()
        oz_server.send_message(internet_request)
        time.sleep(5)


def get_time():
    return str(time.time())


def generate_internet_request():
    return InternetRequest(
        internet_request_type=InternetRequest.InternetRequestTypeEnum.REST,
        internet_request_params={},
        internet_request_endpoint="http://osmankic.home"
    )


if __name__ == '__main__':
    run()
