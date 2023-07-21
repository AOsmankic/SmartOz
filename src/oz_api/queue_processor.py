from collections import deque
import logging
from device_runner.ipc.ipc import IPCRequest, InternetRequest
from threading import Thread

logger = logging.getLogger()

internet_queue: deque = deque()


class QueueProcessor:
    def __init__(self):
        Thread(target=self.process_internet_queue).start()

        print("Queue processors are up and running")

    def process_internet_queue(self):
        global internet_queue
        while True:
            if not bool(internet_queue):
                continue
            else:
                internet_message: InternetRequest = internet_queue.popleft()
                print(internet_message.to_json())

