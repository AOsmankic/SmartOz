from enum import Enum
import json
from typing import Union


def request_from_json(data: Union[bytes, str], is_encoded=True):
    if is_encoded or isinstance(data, bytes):
        data = data.decode()
    object_data = json.loads(data)
    if object_data["IPCRequestType"] == IPCRequestTypes.INTERNET:
        return InternetRequest(from_dict=object_data)


class IPCRequest:

    def __init__(self, request_type: "IPCRequestTypes"):
        self.IPCRequestType = request_type

    def encode(self) -> bytes:
        return self.to_json().encode()

    def to_dict(self) -> dict:
        return self.__dict__.copy()

    def to_json(self) -> str:
        return json.dumps(self.to_dict())

    def from_dict(self, data: dict):
        for key in data:
            setattr(self, key, data[key])


class IPCRequestTypes(str, Enum):
    INTERNET = "INTERNET"



class InternetRequest(IPCRequest):
    class InternetRequestTypeEnum(str, Enum):
        REST = "REST"

    def __init__(
            self,
            internet_request_type: "InternetRequestTypeEnum" = None,
            internet_request_endpoint: "str" = None,
            internet_request_params: "dict" = None,
            from_dict: dict = None
    ):
        if from_dict:
            self.from_dict(from_dict)
            return
        self.request_type = internet_request_type
        self.request_endpoint = internet_request_endpoint
        self.request_params = internet_request_params
        super().__init__(IPCRequestTypes.INTERNET)



