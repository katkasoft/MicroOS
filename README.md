# MicroOS

A minimalist Linux-based operating system built from scratch or lightweight operation on low-spec hardware.

## Features

- **Minimal footprint**: ~37 MB ISO, requires only 125 MB RAM and 40 MB disk space
- **Custom components**: Independent implementation of core system utilities
- **Linux kernel**: Based on Linux 6.18.5-arch1-1
- **Boot system**: GRUB bootloader with UEFI support
- **Init system**: Custom minimal init process written in C
- **Terminal**: Rust-based interactive shell with command execution
- **File system**: Complete directory structure with essential utilities

## System Requirements

- **Architecture**: x86_64 (64-bit)
- **RAM**: 125 MB minimum
- **Storage**: 40 MB minimum for system
- **Boot media**: USB drive (1GB+ recommended)
- **Graphics**: Text mode interface only

## Quick Start

### Download
Get the latest ISO from the [releases page](https://github.com/katkasoft/MicroOS/releases) or from the project website.

### Create Bootable USB

**Linux/macOS:**
```bash
dd if=microos-v0.1.iso of=/dev/sdX bs=4M status=progress
```
Replace `/dev/sdX` with your USB device (e.g., `/dev/sdb`).

**Windows:**
Use [Rufus](https://rufus.ie/) or [Balena Etcher](https://www.balena.io/etcher/).

### Boot and Use
1. Insert USB drive and boot from it
2. MicroOS will start automatically in text mode
3. Type `help` or `commands` for available commands
4. Type `poweroff` to shutdown

## Available Commands
```
MicroOS v0.1 help. Commands:

Term:
clear - clear terminal
exit - exit term
pcd - print current directory
gtd [path] - go to directory

System:
poweroff - power off computer
restart - restart computer
mount [disk] [path] - mount disk

Info:
sys-stat [args] - get information about usage of resources
os-info [args] - get information about OS version and kernel
hw-info [args] - get information about hardware

Files:
create file [filename] - create file
read [file] - read file content
write [file] - write to file
copy [file] - copy file
paste [file] - paste file
cut [file] - cut file to move it
rename [oldname] [newname] - rename file
del [file] - delete file
moveto [file] [path] - move file
chperm [file] [permissions] - change permissions of file
file-stat [file] [args] - show file stats
find [file] - find file

Directories:
sd [dir] [args] - scan directory
rename [oldname] [newname] - rename directory
create dir [name] - create directory
copydir [dir] [destitation] - copy direcrory
movedir [dir] [destitation] - move directory
deldir [dir] - delete directory
chperm [dir] [permissions] - change permissions of directory
find [dir] - find directory

Processes:
stop [pid] [sig] - stop process
lp - list processes

To see all type sd /bin or sd /usr/bin
```

## Development Status

**Current version**: v0.1 (Febrary 2026)

**What works:**
- Basic boot process
- Terminal interface
- Core file operations
- System information utilities
- Process listing

## Contributing

This is currently a personal pet project. Issues and suggestions are welcome through GitHub Issues.

## License

Source code is provided under open-source licenses as indicated in individual source files. Linux kernel components are under GPLv2.

## Disclaimer

MicroOS is an pet project and not intended for production use. Certain features documented on the website may be planned but not yet implemented. Always verify functionality with actual release notes.

## Links

- **Website**: https://katkasoft.github.io/microos
- **Source**: https://github.com/katkasoft/MicroOS
- **Issues**: https://github.com/katkasoft/MicroOS/issues

---

*Linux is a registered trademark of Linus Torvalds. MicroOS is not affiliated with or endorsed by the Linux Foundation.*