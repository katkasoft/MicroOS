use std::io::{self, Write, Read};
use std::fs::File;
use std::process::Command;
use std::path::Path;
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;

fn main() {
    println!("--- MicroOS Term v0.2.1 ---");

    let path_dirs = ["/bin", "/sbin", "/usr/bin", "/usr/sbin"];
    
    let interrupted = Arc::new(AtomicBool::new(false));
    let i = Arc::clone(&interrupted);
    ctrlc::set_handler(move || {
        i.store(true, Ordering::SeqCst);
    }).ok();

    loop {
        interrupted.store(false, Ordering::SeqCst);
        let current_dir = std::env::current_dir().unwrap();
        print!("term {}> ", current_dir.display());
        io::stdout().flush().expect("term: std error!");

        let mut input = String::new();
        io::stdin()
            .read_line(&mut input)
            .expect("term: error while reading input!");

        let parts: Vec<&str> = input.trim().split_whitespace().collect();
        if parts.is_empty() { continue; }

        let cmd_name = parts[0];
        let args = &parts[1..];

        if cmd_name == "exit" {
            break;
        } else if cmd_name == "clear" {
            print!("{esc}[2J{esc}[H", esc = 27 as char);
            io::stdout().flush().expect("flush failed");
            continue;
        } else if cmd_name == "gtd" {
            if !args.is_empty() {
                if let Err(e) = std::env::set_current_dir(args[0]) {
                    println!("gtd: error: {}", e);
                }
            } else {
                println!("gtd: missing path");
            }
            continue;
        } else if cmd_name == "pcd" {
            println!("{}", current_dir.display());
            continue;
        } else if cmd_name == "help" || cmd_name == "commands" {
            match File::open("/etc/help.txt") {
                Ok(mut file) => {
                    let mut content = String::new();
                    if let Ok(_) = file.read_to_string(&mut content) {
                        println!("{}", content);
                    }
                }
                Err(_) => println!("term: help file not found in /etc/help.txt"),
            }
            continue;
        }

        let mut target_path = cmd_name.to_string();
        if !cmd_name.starts_with('/') {
            for dir in path_dirs {
                let p = Path::new(dir).join(cmd_name);
                if p.exists() {
                    target_path = p.to_string_lossy().into_owned();
                    break;
                }
            }
        }

        let child = Command::new(&target_path)
            .args(args)
            .spawn();

        match child {
            Ok(mut process) => {
                loop {
                    match process.try_wait() {
                        Ok(Some(_status)) => break,
                        Ok(None) => {
                            if interrupted.load(Ordering::SeqCst) {
                                let _ = process.kill();
                                break;
                            }
                            std::thread::sleep(std::time::Duration::from_millis(50));
                        }
                        Err(_) => break,
                    }
                }
            }
            Err(_) => {
                println!("term: command '{}' not found. (searched as: {})", cmd_name, target_path);
            }
        }
    }
}