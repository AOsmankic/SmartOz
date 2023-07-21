import uuid

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



    def __init__(self):
        ""



    def get_available_id(self):
        "Do some real cool magic here"
        return uuid.uuid4()


    def construct_partial_view(self):
        "Get nearby nodes"

    def construct_full_view(self):
        "Get all nodes"


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


class NetworkDataPacket:
    data_size = -1
    def __init__(self):
        ""

    def generate_hash_of_data(self):
        ""

    def get_data_info(self):
        ""


class DiscoveryDataPacket(NetworkDataPacket):
    def __init__(self):
        ""
        super().__init__()


class ExternalRequestDataPacket(NetworkDataPacket):
    def __init__(self):
        super().__init__()


class StatusUpdateDataPacket(NetworkDataPacket):
    def __init__(self):
        super().__init__()



