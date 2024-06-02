import pyshark
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter

def load_packets(filename):
    cap = pyshark.FileCapture(filename, display_filter='udp')
    packets = []
    for packet in cap:
        if hasattr(packet.udp, 'payload'):
            packets.append({
                'timestamp': float(packet.sniff_timestamp),
                'data': packet.udp.payload,
            })
    cap.close()
    return pd.DataFrame(packets)

def calculate_latency(sender_df, receiver_df):
    merged_df = pd.merge(sender_df, receiver_df, on='data', suffixes=('_sender', '_receiver'))
    merged_df['latency'] = (merged_df['timestamp_receiver'] - merged_df['timestamp_sender']) * 1000
    return merged_df

def calculate_packet_loss(sender_df, receiver_df):
    merged_df = pd.merge(sender_df, receiver_df, on='data', how='left', indicator=True)
    lost_packets = merged_df[merged_df['_merge'] == 'left_only']
    packet_loss_rate = len(lost_packets) / len(sender_df)
    return lost_packets, packet_loss_rate

def format_func(value, tick_number):
    return f'{int(value)}'

# put files under /latency/data/
sender_packets = load_packets('./data/sender_capture.pcap')
receiver_packets = load_packets('./data/receiver_capture.pcap')

latency_df = calculate_latency(sender_packets, receiver_packets)

lost_packets, packet_loss_rate = calculate_packet_loss(sender_packets, receiver_packets)

average_latency = latency_df['latency'].mean()

print("Statistical Analysis of Latency (ms):")
print(latency_df['latency'].describe())

print("\nPacket Loss Analysis:")
print("Total Packets Sent: {}".format(len(sender_packets)))
print("Total Packets Received: {}".format(len(receiver_packets)))
print("Total Packets Lost: {}".format(len(lost_packets)))
print("Packet Loss Rate: {:.2%}".format(packet_loss_rate))

plt.figure(figsize=(15, 8))
plt.plot(latency_df['timestamp_sender'], latency_df['latency'], marker='o', linestyle='-', color='b', alpha=0.5, markersize=3, label='Latency')
plt.xlabel('Timestamp (Sender) (milliseconds since epoch)')
plt.ylabel('Latency (milliseconds)')
plt.title('Latency Over Time')
plt.grid(True)
plt.legend()

plt.gca().xaxis.set_major_formatter(FuncFormatter(format_func))

latency_df['rolling_mean_latency'] = latency_df['latency'].rolling(window=50).mean()
plt.plot(latency_df['timestamp_sender'], latency_df['rolling_mean_latency'], color='r', label='Rolling Mean Latency')
plt.legend()

plt.show()

print("\nAverage Latency: {:.6f} milliseconds".format(average_latency))

