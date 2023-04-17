# Aple

<h1>Library to attach USB device to WSL - usbipd</h1>
<h2>Commands in PowerShell:</h2>
<ul style="font-size: 30px">
    <li>List all available devices:   <code>usbipd wsl list</code></li>
    <li>Attach new device by bus id:   <code>usbipd wsl attach --busid {busid}</code></li>
    <li>Deattach certain device by bus id:   <code>usbipd wsl detach --busid {busid}</code></li>
</ul>
<h2>Commands in Linux Terminal (WSL):</h2>
<ul style="font-size:30px">
    <li>List attached USB devices:   <code>lsusb</code></li>
</ul>

<h1>Simply usefull commands</h1>
<ul style="font-size: 30px">
    <li>Give serial port R/W rights:   <code>chmod g+rw ttyACM0</code></li>
    <li>Establish direct connection to the serial port:   <code>cu -l /dev/ttyACM0 -s 115200</code></li>
    <li>Check current state of the serial port:   <code>ls -l /dev/ttyACM0</code></li>

<h1>Funny gif haha</h1>
<p align="center">
  <img src="demo.gif" alt="animated" />
</p>
