#include "serial.h"

void set_tty_flags(struct termios *tty) {
    tty->c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty->c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty->c_cflag &= ~CSIZE; // Clear all bits that set the data size
    tty->c_cflag |= CS8; // 8 bits per byte (most common)
    tty->c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty->c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty->c_lflag &= ~ICANON;
    tty->c_lflag &= ~ECHO; // Disable echo
    tty->c_lflag &= ~ECHOE; // Disable erasure
    tty->c_lflag &= ~ECHONL; // Disable new-line echo
    tty->c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty->c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty->c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty->c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    tty->c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty->c_cc[VMIN] = 10;


    cfsetispeed(tty, B115200);
    cfsetospeed(tty, B115200);
}