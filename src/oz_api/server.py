import socket
import os
import sys
import logging
from collections import deque
import threading
from device_runner.ipc.ipc import IPCRequest, request_from_json
from oz_api.queue_processor import internet_queue

logger = logging.getLogger("RunnerComm")
logger.addHandler(logging.StreamHandler(sys.stdout))

socket_path = "/home/osmankic/runner"
DEFAULT_DATA_COMM_SIZE = 1024


class OZInnerServer:
    def __init__(self):
        try:
            os.unlink(socket_path)
        except Exception as e:
            print(e)
            logger.warning("Got error init socket")
            logger.exception(e)

        self.message_queue = deque()

        self.server = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        self.server.bind(socket_path)
        self.server.listen(1)
        print("Server is up and listening for UNIX messages")
        threading.Thread(target=self._retrieve_messages).start()

    def _retrieve_messages(self):
        self.connection, self.client_address = self.server.accept()
        logger.info(f'Connection from {str(self.connection).split(", ")[0][-4:]}')
        try:
            while True:
                data = self.connection.recv(DEFAULT_DATA_COMM_SIZE)

                self.message_queue.append(data.decode())

        finally:
            self.connection.close()
            os.unlink(socket_path)

    def process_messages(self):
        while True:
            if not bool(self.message_queue):
                continue
            message = self.message_queue.popleft()
            print(message)
            data = request_from_json(message, is_encoded=False)
            internet_queue.append(data)







