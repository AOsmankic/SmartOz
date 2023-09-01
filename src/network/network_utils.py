import hashlib


def pad_bytes(data: bytes, total_length: int):
    return data.ljust(total_length, b"\0")


def generate_sha1_hash_digest(data: str) -> str:
    m = hashlib.sha1()
    m.update(data)
    return m.hexdigest()
