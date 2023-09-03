import os
import sys
from unittest import TestCase
from application_layer import ApplicationDataPacket, ApplicationDataPacketTypes

class PacketValidation(TestCase):

    def test_print_packet(self):
        packet = ApplicationDataPacket(
            packet_type=ApplicationDataPacketTypes.EXTERNAL_REQUEST,
            data="Sah"
        )
        byte_output = packet.to_bytes()
        tmp_packet = ApplicationDataPacket(packet_json=byte_output)
        print(tmp_packet.data)


