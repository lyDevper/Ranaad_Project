from usb4a import usb

def list_usb_devices():
    devices = usb.get_usb_device_list()
    if not devices:
        print("No USB devices found.")
    else:
        for device in devices:
            print(f"Device Name: {device.getDeviceName()}")

list_usb_devices()




'''
import serial.tools.list_ports

ports = serial.tools.list_ports.comports()
for port in ports:
    print(f"Port: {port.device}")
'''
