import oz_api.server
import oz_api.queue_processor
import subprocess
import threading
import time

def run():
    oz_api.queue_processor.QueueProcessor()
    threading.Thread(target=get_messages).start()
    # time.sleep(5)
    launch_runner()


def get_messages():
    oz_server = oz_api.server.OZInnerServer()
    oz_server.process_messages()

def launch_runner():
    subprocess.run(["python3", "device_runner/runner.py"])

if __name__ == '__main__':
    run()
