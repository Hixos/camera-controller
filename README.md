# Camera Controller
## Remote DSLR control with Raspberry PI and Android

### Gphoto2 problems
#### "could not claim the usb device" error

This is due to the `gvfs-gphoto2-volume-monitor` process.   
Run `ps aux | grep gphoto` and kill the process using `sudo kill -9 <pid>`.

### Setup build system
`./setup.sh`

### Compile
 - `meson compile -C builddir/arm`  
 or  
 - `meson compile -C builddir/debug`

## Make Raspberry Pi discoverable by Android app
1. Change host name to `cameracontroller`  in `/etc/hosts`         
2. Change host name to `cameracontroller`  in `/etc/hostname`     
3. Apply changes with `sudo /etc/init.d/hostname.sh`
4. Set `publish-workstation=yes` in `/etc/avahi/avahi-daemon.conf`

## Cross-compiling

### Toolchain
See https://github.com/abhiTronix/raspberry-pi-cross-compilers

### Rsync sysroot
Run `scripts/do_rsync` in a desired folder (eg. `/opt/cross-pi-gcc-10.3.0-1`) (remember to update ssh username and key path)
