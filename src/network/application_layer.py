from oz_network import PacketException
from typing import Optional, Union
import json
from oz_network import MAX_PACKET_LENGTH, OzPacket
from network_utils import pad_bytes, generate_sha1_hash_digest, bytes_to_ints
from enum import Enum


APP_PACKET_HEADER_LENGTH = 23


class ApplicationDataPacket(OzPacket):
    def __init__(
            self,
            packet_json: Optional[Union[dict, str]] = None,
            *,
            packet_type: "ApplicationDataPacketTypes" = None,
            data: str = None
    ):
        super().__init__()
        if packet_json:
            if isinstance(packet_json, dict):
                self.from_dict(packet_json)
            else:
                self.from_json(packet_json)
            self.validate_packet()
            return
        self.application_data_type = packet_type.value
        self.data = data
        self.data_len = len(self.data)
        self.checksum = self._generate_hash_of_data()
        self.validate_packet()

    def validate_packet(self):
        if not isinstance(self.application_data_type, int) or 0 > self.application_data_type > 5:
            raise InvalidApplicationPacketException(f"Invalid packet type {str(self.application_data_type)}")
        if not isinstance(self.data, str):
            raise InvalidApplicationPacketException("Packed data must be str")
        if self.data_len >= MAX_PACKET_LENGTH - APP_PACKET_HEADER_LENGTH:
            raise InvalidApplicationPacketException(f"Invalid packet length of {str(len(self.data))}")

    def validate_packet_integrity(self):
        """
        Compare the hash of the current packet data with the checksum header.
        :return: True if valid integrity, exception if invalid.
        :raises: ApplicationPacketIntegrityException
        """
        curr_data_hash = self._generate_hash_of_data()
        given_hash = self.checksum
        if curr_data_hash != given_hash:
            raise ApplicationPacketIntegrityException()
        return True

    def to_bytes(self):
        """
        Convert packet to bytes and pad headers as needed
        :return: bytes representing the packet
        """
        byte_array = []

        byte_array.extend(bytes_to_ints(self.application_data_type.to_bytes(1, 'big')))
        byte_array.extend(bytes_to_ints(self.data_len.to_bytes(2, 'big')))
        byte_array.extend(pad_bytes(bytes(self.checksum, 'utf-8'), 40))
        byte_array.extend(bytes_to_ints(bytes(self.data, 'utf-8')))

        return byte_array


class ApplicationDataPacketTypes(Enum):
    # Types of application packets
    STATUS_UPDATE = 0
    EXTERNAL_REQUEST = 1
    INTERNAL_REQUEST = 2
    DISCOVERY_REQUEST = 3


class ApplicationPacketException(PacketException):
    pass


class ApplicationPacketIntegrityException(ApplicationPacketException):
    def __init__(self, msg: Optional[str] = ""):
        self.msg = f"Packet has invalid checksum. {msg if msg else ''}"
        super().__init__(self.msg)


class InvalidApplicationPacketException(ApplicationPacketException):
    def __init__(self, msg: str):
        self.msg = msg
        super().__init__(self.msg)