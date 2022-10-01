import struct
from time import sleep, time

from min import MINTransportSerial

# Set your serial port name here. 
MIN_PORT = "COM12"

def wait_for_frames(min_handler: MINTransportSerial, timeoutMs:int):
    while True:
        if timeoutMs > 0:    
            frames = min_handler.poll()
            if frames:
                return frames
        else:
            return []
        sleep(0.1)        
        timeoutMs -= 100

def print_rx_frame(min_id, payload):
    if min_id == 1:
        # parse frame contents.
        flowCondition = struct.unpack_from("<I", payload, 16)[0]
        temperature = struct.unpack_from("<i", payload, 20)[0]
        flowRate = struct.unpack_from("<f", payload, 24)[0]
        print("\tFlow condition ", flowCondition)
        print("\tTemperature ", temperature, "°C")
        print("\tFlow rate ", flowRate)

min_handler = MINTransportSerial(port=MIN_PORT)
min_handler._serial.baudrate = 9600

while True:
    payload = bytes([0x00])
    print("Sending read request...")
    # Send a frame on the serial line
    min_handler.send_frame(min_id=0x01, payload=payload)

    # Wait for one or more frames to come back from the serial line and print them out
    for frame in wait_for_frames(min_handler, 2000):
        print("Rx frame: MIN-ID ", frame.min_id, "Len = ", len(frame.payload))
        print_rx_frame(frame.min_id, frame.payload)
    # Wait a little bit so we don't flood the other end
    sleep(1)
