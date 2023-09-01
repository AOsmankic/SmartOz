import enum
import json
import uuid
import hashlib
from typing import Optional, Union
from network_utils import generate_sha1_hash_digest

MAX_PACKET_LENGTH = 65535


class NeighborNode:
    node_id = ""
    date_added = None
    signal_strength = -1
    lan_capable = False

    def __init__(self):
        ""

def is_for_me():
    "👉👈"





class OzNetwork:
    network_id = None
    network_nodes = []
    unique_id = None

    def __init__(self):
        ""


    def get_available_id(self):
        "Do some real cool magic here"
        return uuid.uuid4()


    def construct_partial_view(self):
        "Get nearby nodes"

    def construct_full_view(self):
        "Get all nodes"



class OzPacket:
    def __init__(self):
        pass

    def to_dict(self):
        return self.__dict__.copy()

    def to_json(self):
        return json.dumps(self.to_dict())

    def from_dict(self, data: dict):
        self.__dict__ = data.copy()

    def from_json(self, data: str):
        data_dict = json.loads(data)
        self.from_dict(data_dict)

    def _generate_hash_of_data(self):
        return generate_sha1_hash_digest(self.data)

class NetworkPacket:
    packet_id = ""
    source_id = ""
    destination_id = ""
    request_type = None
    last_holder = ""
    data_size = ""
    data_hash = ""
    hops = 0
    data = ""
    path = []

    def __init__(self):
        ""



class DiscoveryDataPacket(ApplicationDataPacket):
    def __init__(self):
        ""
        super().__init__()


class ExternalRequestDataPacket(ApplicationDataPacket):
    def __init__(self):
        super().__init__()


class StatusUpdateDataPacket(ApplicationDataPacket):
    def __init__(self):
        super().__init__()


class PacketException(Exception):
    pass
