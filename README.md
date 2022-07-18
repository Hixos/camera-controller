# Camera Controller 3
## Remote DSLR control with Raspberry PI and Android

### Gphoto2 problems
#### "could not claim the usb device" error

This is due to the `gvfs-gphoto2-volume-monitor` process.   
Run `ps aux | grep gphoto` and kill the process using `kill -9 <pid>`.

### Compile
 - `meson setup builddir/debug --buildtype debug`
 - `meson compile -C builddir/debug`

### Cross Compile
 - `meson setup builddir/arm --cross-file arm-linux-gnueabihf.ini`
 - `meson compile -C builddir/arm`
