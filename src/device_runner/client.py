import socket
import os
import sys
import logging
from ipc.ipc import IPCRequest

logger = logging.getLogger("OzConnection")
logger.setLevel(logging.DEBUG)
logger.addHandler(logging.StreamHandler(sys.stdout))


socket_path = "/home/osmankic/runner"

DEFAULT_DATA_COMM_SIZE = 1024


class OzInnerClient:
    def __init__(self):
        self.client = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        self.client.connect(socket_path)
        logger.info("Client successfully connected to server")

    def send_message(self, data: IPCRequest):
        self.client.sendall(data.encode())

    def close(self):
        self.client.close()

