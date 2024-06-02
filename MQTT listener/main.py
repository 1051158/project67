import paho.mqtt.client as mqtt
import subprocess
import os

# Configuration settings for MQTT and streaming
mqtt_broker_host = ""
mqtt_port = 1883
target_host = "" # This is the IP of the device we're streaming to
target_port = 5000
starting_bitrate = 500
username = ""
password = ""
topic = ""
path_to_stream_exe = "..\stream\stream"
 
stream_pid = None

def on_connect(client, userdata, flags, rc):
    print("Connected with result code:", rc)
    client.subscribe(topic)

def on_message(client, userdata, msg):
    global stream_pid
    command = msg.payload.decode("utf-8").split(':')
    match command[0]: 
        case "cameraOn":
            if stream_pid is None:
                print("Starting camera stream...")
                process = subprocess.Popen(
                    [path_to_stream_exe, target_host, str(target_port), str(starting_bitrate)],
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    text=True
                )
                stream_pid = process.pid
                print(f"Stream started with PID {stream_pid}")
            else:
                print("Stream is already running.")
        case "cameraOff":
            if stream_pid is not None:
                print(f"Stopping camera stream with PID {stream_pid}...")
                os.kill(stream_pid, 9)
                stream_pid = None
                print("Stream stopped.")
            else:
                print("Stream process is not running.")
        case "toggleColor":
            print(f"Toggling color mode to {command[1]}")
            subprocess.Popen(
                f'echo {command[1]} > /tmp/color_toggle_fifo', 
                shell=True, 
                stdout=subprocess.PIPE, 
                stderr=subprocess.PIPE, 
                text=True
            ).communicate()
        case "setBitrate":
            print(f"Setting bitrate to {command[1]}")
            subprocess.Popen(
                f'echo {command[1]} > /tmp/bitrate_fifo', 
                shell=True, 
                stdout=subprocess.PIPE, 
                stderr=subprocess.PIPE, 
                text=True
            ).communicate()
        case _:
            error = "Command not supported"
            print(error)

def on_publish(client, userdata, mid, reason_code, properties):
    print("Data published")

if not os.path.exists('/tmp/bitrate_fifo'):
    os.mkfifo('/tmp/bitrate_fifo')

if not os.path.exists('/tmp/color_toggle_fifo'):
    os.mkfifo('/tmp/color_toggle_fifo')

client = mqtt.Client()

client.on_connect = on_connect
client.on_message = on_message
client.on_publish = on_publish

client.username_pw_set(username=username, password=password)

print("Connecting...")
client.connect(mqtt_broker_host, mqtt_port)

client.loop_forever()

