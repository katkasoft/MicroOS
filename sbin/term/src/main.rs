use std::io::{self, Write, Read, BufRead, BufReader};
use std::fs::File;
use std::process::{Command, exit};
use std::path::Path;
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;
use std::env;
use rustyline::error::ReadlineError;
use rustyline::DefaultEditor;
use std::thread::sleep;
    use std::time::Duration;

fn exec_command(cmd_name: &str, args: &[&str], interrupted: &Arc<AtomicBool>, is_interactive: bool) -> bool {
    let path_dirs = ["/bin", "/sbin", "/usr/bin", "/usr/sbin"];

    match cmd_name {
        "exit" => return false,

        "kernel-panic" => {
            if !is_interactive {
                println!("kernel-panic: only available in interactive mode.");
                return true;
            }

            print!("Are you sure you want to trigger kernel panic? (y/N): ");
            io::stdout().flush().unwrap();
            
            let mut answer = String::new();
            io::stdin().read_line(&mut answer).unwrap();
            
            if answer.trim().to_lowercase() == "y" {
                exit(2);
            }
        }

        "echo" => {
            println!("{}", args.join(" "));
        }

        "clear" => {
            print!("{esc}[2J{esc}[H", esc = 27 as char);
            let _ = io::stdout().flush();
        }

        "gtd" => {
            if !args.is_empty() {
                if let Err(e) = std::env::set_current_dir(args[0]) {
                    println!("gtd: error: {}", e);
                }
            } else {
                println!("gtd: missing path");
            }
        }

        "pcd" => {
            if let Ok(current_dir) = std::env::current_dir() {
                println!("{}", current_dir.display());
            }
        }

        "help" | "commands" => {
            match File::open("/etc/help.txt") {
                Ok(mut file) => {
                    let mut content = String::new();
                    if file.read_to_string(&mut content).is_ok() {
                        println!("{}", content);
                    }
                }
                Err(_) => println!("term: help file not found in /etc/help.txt"),
            }
        }

        _ => {
            let mut target_path = cmd_name.to_string();
            if !cmd_name.starts_with('/') && !cmd_name.starts_with("./") {
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
                                    println!("^C (killed)");
                                    break;
                                }
                                std::thread::sleep(std::time::Duration::from_millis(50));
                            }
                            Err(_) => break,
                        }
                    }
                }
                Err(_) => {
                    println!("term: command '{}' not found.", cmd_name);
                }
            }
        }
    }
    true
}

fn main() {
    let args_os: Vec<String> = env::args().collect();
    let interrupted = Arc::new(AtomicBool::new(false));
    let i = Arc::clone(&interrupted);
    ctrlc::set_handler(move || {
        i.store(true, Ordering::SeqCst);
    }).ok();

    if args_os.len() == 2 {
        let mut start_mode = false;
        if args_os[1] == "start" {
            start_mode = true;
            println!("[START]: running start script...");
        }
        let mut file_path;
        if (start_mode) {
            file_path = "/sbin/start.msh";
        } else {
            file_path = &args_os[1];
        }
        let file = match File::open(file_path) {
            Ok(f) => f,
            Err(e) => {
                if (start_mode) {
                    eprintln!("[START]: fatal error: could not open /sbin/start.msh: {}. System is unable to load.", e);
                    sleep(Duration::from_secs(5));
                } else {
                    eprintln!("term: could not open script {}: {}", file_path, e);
                }
                return;
            }
        };
        
        let reader = BufReader::new(file);
        for line_result in reader.lines() {
            if let Ok(line) = line_result {
                if start_mode {
                    println!("[START]: executing {}", line);
                }
                let trimmed = line.trim();
                if trimmed.is_empty() || trimmed.starts_with('#') { continue; }

                let parts: Vec<&str> = trimmed.split_whitespace().collect();
                if !exec_command(parts[0], &parts[1..], &interrupted, false) {
                    if start_mode {
                        eprintln!("[START]: error while executing {}", line);
                    }
                    break;
                }
            }
        }
    } else {
        println!("--- MicroOS v0.1 ---");
        println!("--- MicroOS Term v0.4 ---");

        let mut rl = match DefaultEditor::new() {
            Ok(editor) => Some(editor),
            Err(_) => {
                println!("term: [WARNING] Kernel lacks TTY support. Arrows/History disabled.");
                None
            }
        };

        loop {
            interrupted.store(false, Ordering::SeqCst);
            let current_dir = env::current_dir().unwrap_or_else(|_| Path::new("/").to_path_buf());
            let prompt = format!("term {}> ", current_dir.display());

            let input = if let Some(ref mut editor) = rl {
                match editor.readline(&prompt) {
                    Ok(line) => {
                        if !line.trim().is_empty() {
                            let _ = editor.add_history_entry(line.as_str());
                        }
                        line
                    },
                    Err(ReadlineError::Interrupted) => {
                        println!("^C");
                        continue;
                    },
                    Err(ReadlineError::Eof) => break,
                    Err(err) => {
                        println!("term: readline error: {:?}", err);
                        break;
                    }
                }
            } else {
                print!("{}", prompt);
                io::stdout().flush().expect("term: flush error");
                let mut line = String::new();
                if io::stdin().read_line(&mut line).is_err() {
                    break;
                }
                line
            };

            let parts: Vec<&str> = input.trim().split_whitespace().collect();
            if parts.is_empty() { continue; }

            if !exec_command(parts[0], &parts[1..], &interrupted, true) {
                break;
            }
        }
    }
}