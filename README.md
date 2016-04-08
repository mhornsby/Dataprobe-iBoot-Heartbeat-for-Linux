# Dataprobe-iBoot-Heartbeat-for-Linux

  Program to generate heartbeat signal for Dataprobe (www.dataprobe.com )
  iBoot remote power boot over internet device.
 
  This program sends a datapacket to the iBoot at a defined interval. Should the
  iBoot stop recieving packets the iBoot can be configured via its web interface to reboot
  equipment or perform other power switching functions.
 
  Usage:
  heartbeat -h hostname [-p port] [-r secs] [-d]
  
  -h hostname. Host name or IP address of iBoot<br>
  -p port. UDP port default port is 9100<br>
  -r time in seconds to repeat messages in seconds. Default 10 seconds. 0 means run once only.<br>
  -d enable diagnostic messages<br>
