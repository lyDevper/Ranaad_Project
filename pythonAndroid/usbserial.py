import usbserial

ports = usbserial.list_ports()
print("Available Ports:", ports)

if ports:
    ser = usbserial.Serial(ports[0], 115200)
    print("Connected to Arduino!")
else:
    print("No ports found.")
