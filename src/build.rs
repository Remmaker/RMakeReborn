use crate::config::*;

#[derive(Default, Debug)]
pub struct BuildConfig {
    compiler: String,
    flags: Option<Vec<String>>,
    src: Vec<String>,
    include: Option<Vec<String>>,
    lflags: Option<Vec<String>>,
    lpaths: Option<Vec<String>>,
    target: String
}


pub fn parse_build(conf: &Config) -> Result<BuildConfig, ConfigError> {

    let mut build_conf: BuildConfig = BuildConfig::default();
    let hash_build = conf.section.get("build");
    let build = hash_build.unwrap();
    
    if !build.contains_key("compiler") || !build.contains_key("src") {
        return Err(ConfigError::InvalidConfig { message: "Missing 'compiler' and/or 'src' key/value to execute build".into() });
    }

    for (k, v) in build.iter() {
        match k.as_str() {
            "compiler" => {
                if v.split_once(" ").is_some() {
                    return Err(ConfigError::InvalidConfig { message: "Only one compiler is supported at time".into() })
                }
                build_conf.compiler = v.to_string(); 
            },
            "flags" => {
                build_conf.flags.get_or_insert_with(Vec::new).extend(v.split_whitespace().map(|s| s.to_string()));
            },
            "src" => {
                build_conf.src.extend(v.split_whitespace().map(|s| s.to_string()));
            },
            "include" => {
                build_conf.include.get_or_insert_with(Vec::new).extend(v.split_whitespace().map(|s| s.to_string()));
            },
            "lflags" => {
                build_conf.lflags.get_or_insert_with(Vec::new).extend(v.split_whitespace().map(|s| s.to_string()));
            },
            "lpaths" => {
                build_conf.lpaths.get_or_insert_with(Vec::new).extend(v.split_whitespace().map(|s| s.to_string()));
            },
            "target" => {
                if v.split_once(" ").is_some() {
                    return Err(ConfigError::InvalidConfig { message: "Only one target is supported at time".into() })
                }
                build_conf.target = v.to_string();
            },
            _ => {
                eprintln!("Warning: Unknow keyword '{k}'");
            }  
        }
    }

    Ok(build_conf)
}

pub fn execute_build(conf: &BuildConfig) -> Result<Vec<CmdOutput>, ConfigError> {
    let compiler = conf.compiler.clone();
    let is_cl = compiler.ends_with("cl") || compiler.ends_with("cl.exe");

    let output = std::process::Command::new(compiler)
        .args(conf.flags.clone().get_or_insert_with(Vec::new).iter())
        .args(conf.include.clone().get_or_insert_with(Vec::new).iter()
            .map(|s| if !is_cl { format!("-I{s}") } else { format!("/I {s}")}))
        .args(conf.src.iter())
        .arg(if !is_cl { "-o" } else { "/o" }) 
        .arg(conf.target.clone()) 
        .args(conf.lpaths.clone().get_or_insert_with(Vec::new).iter()
            .map(|s| if !is_cl { format!("-L{s}") } else { format!("/L {s}") }))
        .args(conf.lflags.clone().get_or_insert_with(Vec::new).iter() 
            .map(|s| if !is_cl { format!("-{s}") } else { s.to_string() }))
        .output()
            .map_err(|_| ConfigError::CommandFailed { cmd: conf.compiler.clone(), message: "Unexpected".into() })?;

    let mut ret: Vec<CmdOutput> = Vec::new();
    let tmp: CmdOutput = CmdOutput { 
        stdout: String::from_utf8_lossy(&output.stdout).to_string(), 
        stderr: String::from_utf8_lossy(&output.stderr).to_string(), 
        status: output.status 
    };

    ret.push(tmp);
    Ok(ret)
}

