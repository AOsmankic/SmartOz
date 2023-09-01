from unittest import TestCase
from transport_layer import TransportLayerPacket, NetworkPathTypes

class PacketValidation(TestCase):

    def test_from_bytes(self):
        packet = TransportLayerPacket(
            packet_type=NetworkPathTypes.REREQUEST,
            data="Sah",
            source="me",
            dest="you",
            packet_num=0,
            num_of_packets=1,
            packet_id=123
        )
        byte_output = packet.to_bytes()
        print(byte_output)
        tmp_packet = TransportLayerPacket(packet_json=byte_output)
        print(tmp_packet.data)
        self.assertEquals("Sah", tmp_packet.data)

        # print(packet.to_bytes().decode('utf-8'))
        # print(''.join(format(x, '02x') for x in packet.to_bytes()))
