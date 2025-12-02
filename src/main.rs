mod cli;
mod config;
mod run;
mod build;

fn main() {
    if let Err(err) = cli::run() {
        eprintln!("Error {err}");
        std::process::exit(1);
    }
}
