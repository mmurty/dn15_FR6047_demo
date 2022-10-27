import struct
from time import sleep, time

from min import MINTransportSerial

# Set your serial port name here. 
MIN_PORT = "COM9"

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
    # Firmware version command.
    if min_id == 1:
        # parse frame contents.
        fwVer = struct.unpack_from("<25s", payload, 0)[0]
        fwVer = str(fwVer, 'UTF-8')
        hwVer = struct.unpack_from("<15s", payload, 25)[0]
        hwVer = str(hwVer, 'UTF-8')
        srNo = struct.unpack_from("<I", payload, 40)[0]
        diameter = struct.unpack_from("<B", payload, 44)[0]
        print("\tFwVer ", fwVer)
        print("\tHwVer ", hwVer)
        print("\tSrNo ", srNo)
        print("\tDiameter ", diameter)
    # 1Hz parameter commands.
    elif min_id == 4:
        # parse frame contents.
        flowRate = struct.unpack_from("<f", payload, 0)[0]
        flowDir = struct.unpack_from("<B", payload, 4)[0]
        flowOn = struct.unpack_from("<B", payload, 5)[0]
        cumDSG = struct.unpack_from("<Q", payload, 6)[0]
        cumRevDSG = struct.unpack_from("<Q", payload, 14)[0]
        flowCondition = struct.unpack_from("<B", payload, 22)[0]
        temperature = struct.unpack_from("<B", payload, 23)[0]
        
        print("\tFlow rate ", flowRate)
        print("\tFlow direction ", flowDir)
        print("\tFlow On ", flowOn)
        print("\tCumDsg ", cumDSG)
        print("\tCumRevDsg ", cumRevDSG)
        print("\tFlow condition ", flowCondition)
        print("\tTemperature ", temperature, "°C")
        
min_handler = MINTransportSerial(port=MIN_PORT)
min_handler._serial.baudrate = 9600

payload = bytes([0x00])
print("Read Firmware version: ")
# Send a frame on the serial line
min_handler.send_frame(min_id=0x01, payload=payload)
# Wait for one or more frames to come back from the serial line and print them out
for frame in wait_for_frames(min_handler, 5000):
    print("Rx frame: MIN-ID ", frame.min_id, "Len = ", len(frame.payload))
    print_rx_frame(frame.min_id, frame.payload)

while True:
    payload = bytes([0x00])
    print("Send 1Hz read request...")
    # Send a frame on the serial line
    min_handler.send_frame(min_id=0x04, payload=payload)

    # Wait for one or more frames to come back from the serial line and print them out
    for frame in wait_for_frames(min_handler, 1000):
        print("Rx frame: MIN-ID ", frame.min_id, "Len = ", len(frame.payload))
        print_rx_frame(frame.min_id, frame.payload)
    # Wait a little bit so we don't flood the other end
    sleep(1)
