#!/bin/bash

port=5000

if [ $# -gt 0 ]; then
  port=$1
fi

caps='application/x-rtp, media=(string)video, encoding-name=(string)H264'

pipeline="gst-launch-1.0 udpsrc port=$port caps=\"$caps\" ! queue ! rtph264depay ! queue ! h264parse ! queue ! avdec_h264 ! queue ! videoconvert ! queue ! autovideosink"

eval $pipeline
