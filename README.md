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

### System
- `poweroff` - Shutdown system
- `restart` - Reboot system
- `mount` - Mount filesystems

### Information
- `sys-stat` - System resource usage
- `os-info` - OS and kernel information
- `hw-info` - Hardware information

### File Operations
- `create` - Create files/directories
- `read` - Read file contents
- `write` - Write to files
- `copy`/`paste`/`cut` - File clipboard operations
- `rename`/`del`/`moveto` - File management
- `chperm` - Change permissions
- `file-stat` - File statistics

### Directory Operations
- `sd` - Scan directory
- `copydir`/`movedir`/`deldir` - Directory management

### Process Management
- `lp` - List processes
- `stop` - Stop process

### Terminal Built-ins
- `clear` - Clear screen
- `exit` - Exit terminal
- `pcd` - Print current directory
- `gtd` - Change directory


## Development Status

**Current version**: v0.1 (October 2024)

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