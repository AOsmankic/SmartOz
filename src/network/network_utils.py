import hashlib
from typing import List


def pad_bytes(data: bytes, total_length: int) -> List[int]:
    init_list = bytes_to_ints(data)
    final_list = ([0]*(total_length - len(init_list)))
    final_list.extend(init_list)
    return final_list


def bytes_to_ints(data: bytes) -> List[int]:
    final_list = []
    for byte in data:
        final_list.append(byte)
    return final_list


def generate_sha1_hash_digest(data: str) -> str:
    m = hashlib.sha1()
    m.update(data.encode())
    return m.hexdigest()
