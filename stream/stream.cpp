#include <gst/gst.h>
#include <glib.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <memory>
#include <fstream>

std::atomic<int> bitrate;
std::atomic<bool> is_greyscale;
std::shared_ptr<std::atomic<GstElement*>> current_x264enc;

void write_current_color(const std::string& color_mode) {
    std::ofstream color_file("/tmp/current_color", std::ios::out | std::ios::trunc);
    if (color_file.is_open()) {
        color_file << color_mode << std::endl;
        color_file.close();
        std::cout << "Updated /tmp/current_color with value: " << color_mode << std::endl;
    } else {
        std::cerr << "Failed to open /tmp/current_color for writing." << std::endl;
    }
}

void change_bitrate_from_pipe(const char* pipe_path) {
    int fd = open(pipe_path, O_RDONLY);
    if (fd == -1) {
        std::cerr << "Failed to open pipe: " << strerror(errno) << std::endl;
        return;
    }

    char buffer[128];
    while (true) {
        ssize_t count = read(fd, buffer, sizeof(buffer) - 1);
        if (count == -1) {
            std::cerr << "Error reading from pipe: " << strerror(errno) << std::endl;
            break;
        } else if (count > 0) {
            buffer[count] = '\0';
            try {
                int new_bitrate = std::stoi(buffer);
                bitrate.store(new_bitrate);
                GstElement* x264enc = current_x264enc->load();
                if (x264enc) {
                    g_object_set(x264enc, "bitrate", new_bitrate, NULL);
                    std::cout << "Bitrate changed to: " << new_bitrate << " kbps" << std::endl;
                }
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid bitrate value received: " << buffer << std::endl;
            }
        }
    }

    close(fd);
}

void toggle_color_from_pipe(const char* pipe_path) {
    int fd = open(pipe_path, O_RDONLY);
    if (fd == -1) {
        std::cerr << "Failed to open pipe: " << strerror(errno) << std::endl;
        return;
    }

    char buffer[128];
    while (true) {
        ssize_t count = read(fd, buffer, sizeof(buffer) - 1);
        if (count == -1) {
            std::cerr << "Error reading from pipe: " << strerror(errno) << std::endl;
            break;
        } else if (count > 0) {
            buffer[count] = '\0';
            std::string command(buffer, count);
            write_current_color(command);
            if (command == "color\n") {
                is_greyscale.store(false);
            } else if (command == "greyscale\n") {
                is_greyscale.store(true);
            }

            // Signal to recreate the pipeline
            return;
        }
    }

    close(fd);
}

GstElement* create_pipeline(const char *host, int port, int bitrate, bool greyscale) {
    std::string pipeline_str =
        "v4l2src ! "
        "videorate ! "
        "video/x-raw,framerate=30/1 ! "
        "videoscale ! "
        "video/x-raw,width=720,height=480 ! "
        "videoconvert ! ";

    if (greyscale) {
        pipeline_str += "video/x-raw,format=GRAY8 ! videoconvert ! ";
    }

    pipeline_str +=
        "clockoverlay time-format=\"%Y-%m-%d %H:%M:%S\" ! "
        "x264enc tune=zerolatency bitrate=" + std::to_string(bitrate) + " speed-preset=superfast name=enc ! "
        "rtph264pay ! "
        "udpsink host=" + std::string(host) + " port=" + std::to_string(port);

    GError *error = nullptr;
    GstElement *pipeline = gst_parse_launch(pipeline_str.c_str(), &error);
    if (!pipeline) {
        std::cerr << "Failed to create pipeline: " << error->message << std::endl;
        g_error_free(error);
        return nullptr;
    }

    return pipeline;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <IP_ADDRESS> <PORT> <BITRATE>" << std::endl;
        return -1;
    }

    const char *host = argv[1];
    int port = std::stoi(argv[2]);
    bitrate.store(std::stoi(argv[3]));
    is_greyscale.store(false);
    current_x264enc = std::make_shared<std::atomic<GstElement*>>(nullptr);

    gst_init(&argc, &argv);

    GstElement *pipeline = nullptr;
    std::thread bitrate_thread(change_bitrate_from_pipe, "/tmp/bitrate_fifo");

    while (true) {
        if (pipeline) {
            gst_element_set_state(pipeline, GST_STATE_NULL);
            gst_object_unref(pipeline);
        }

        pipeline = create_pipeline(host, port, bitrate.load(), is_greyscale.load());
        if (!pipeline) {
            std::cerr << "Failed to recreate pipeline. Exiting." << std::endl;
            break;
        }

        GstElement *x264enc = gst_bin_get_by_name(GST_BIN(pipeline), "enc");
        if (!x264enc) {
            std::cerr << "Failed to get x264enc element from pipeline." << std::endl;
            gst_object_unref(pipeline);
            break;
        }

        // Update the current x264enc element
        current_x264enc->store(x264enc);

        GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
        if (ret == GST_STATE_CHANGE_FAILURE) {
            std::cerr << "Failed to set pipeline to playing state." << std::endl;

            // Check for error message
            GstBus *bus = gst_element_get_bus(pipeline);
            GstMessage *msg = gst_bus_poll(bus, GST_MESSAGE_ERROR, 1000);
            if (msg) {
                GError *err;
                gchar *debug_info;
                gst_message_parse_error(msg, &err, &debug_info);
                std::cerr << "Error received from element " << GST_OBJECT_NAME(msg->src) << ": " << err->message << std::endl;
                std::cerr << "Debugging information: " << (debug_info ? debug_info : "none") << std::endl;
                g_clear_error(&err);
                g_free(debug_info);
                gst_message_unref(msg);
            }
            gst_object_unref(bus);
            gst_object_unref(pipeline);
            bitrate_thread.join();
            break;
        }

        std::cout << "Streaming to " << host << ":" << port << " with initial bitrate " << bitrate.load() << " kbps" << std::endl;

        // Monitor color toggle pipe in the main loop
        toggle_color_from_pipe("/tmp/color_toggle_fifo");
    }

    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
    }
    if (bitrate_thread.joinable()) {
        bitrate_thread.join();
    }

    return 0;
}

