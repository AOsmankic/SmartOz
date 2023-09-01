from enum import Enum
from typing import Optional, Union, Dict
from network_utils import pad_bytes
from oz_network import OzPacket, PacketException, OzNetwork
import logging
logger = logging.getLogger("ozNetwork")

def transport_layer_retrieval_handler(data: bytes, oz_network: OzNetwork):
    transport_layer_packet = TransportLayerPacket(packet_json=data)
    try:
        transport_layer_packet.validate_packet_integrity()
    except TransportPacketIntegrityException as e:

        logger.warning("Invalid checksum. Attempting to re-request data")
        transport_layer_rerequest_packet = TransportLayerPacket(
            packet_type=NetworkPathTypes.REREQUEST,
            data="",
            source=oz_network.unique_id,
            dest=transport_layer_packet.source,
            packet_num=0,
            num_of_packets=1,
            packet_id=transport_layer_packet.packet_id + 1
        )

def transport_layer_transmit_handler():
    ""

class NetworkPathTypes(int, Enum):
    DISCOVER_NETWORK: 0  # Discover network.
    DISCOVER_NEIGHBORS: 1  # Discover network neighbors
    DISCOVER_ROUTE: 2
    ROUTE: 3
    REREQUEST: 4



class TransportLayerPacket(OzPacket):
    def __init__(
            self,
            packet_json: Optional[Union[Dict, str, bytes]],
            *,
            packet_type: NetworkPathTypes = None,
            data: str = None,
            source: str = None,
            dest: str = None,
            packet_num: int = -1,
            num_of_packets: int = -1,
            packet_id: int = -1
    ):

        super().__init__()
        if packet_json:
            if isinstance(packet_json, dict):
                self.from_dict(packet_json)
            elif isinstance(packet_json, bytes):
                self.from_bytes(packet_json)
            else:
                self.from_json(packet_json)
            return

        self.data = data
        self.data_len = len(data)
        self.source = source
        self.dest = dest
        self.packet_type = packet_type
        self.packet_num = packet_num
        self.num_of_packets = num_of_packets
        self.packet_id = packet_id
        self.checksum = super()._generate_hash_of_data()

    def from_bytes(self, data: bytes):
        self.source = data[0:16].decode()
        self.dest = data[16:32].decode()
        self.data_len = int.from_bytes(data[32:34])
        self.packet_type = int.from_bytes(data[34:35])
        self.packet_id = int.from_bytes(data[35:37])
        self.packet_num = int.from_bytes(data[37:39])
        self.num_of_packets = int.from_bytes(data[39:41])
        self.checksum = data[41:61].decode
        self.data = data[61:].decode()

    def to_bytes(self):
        final_str = [
            f"{pad_bytes(bytes(self.source, 'utf-8'), 16)}",
            f"{pad_bytes(bytes(self.dest, 'utf-8'), 16)}",
            f"{pad_bytes(self.data_len.to_bytes(), 16)}",
            f"{pad_bytes(self.packet_type.to_bytes(), 16)}",
            f"{pad_bytes(self.packet_id.to_bytes(), 16)}",
            f"{pad_bytes(self.packet_num.to_bytes(), 16)}",
            f"{pad_bytes(self.num_of_packets.to_bytes(), 16)}",
            f"{pad_bytes(bytes(self.checksum, 'utf-8'), 16)}",
            f"{self.data}"
        ]
        final_str = "".join(final_str)
        return bytes(final_str, "utf-8")

    def validate_packet_integrity(self):
        """
        Compare the hash of the current packet data with the checksum header.
        :return: True if valid integrity, exception if invalid.
        :raises: ApplicationPacketIntegrityException
        """
        curr_data_hash = self._generate_hash_of_data()
        given_hash = self.checksum
        if curr_data_hash != given_hash:
            raise TransportPacketIntegrityException()
        return True


class TransportPacketException(PacketException):
    pass

class TransportPacketIntegrityException(TransportPacketException):
    def __init__(self, msg: Optional[str] = ""):
        self.msg = f"Packet has invalid checksum. {msg if msg else ''}"
        super().__init__(self.msg)
