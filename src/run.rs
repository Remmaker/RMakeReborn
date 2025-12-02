use crate::config::*;

#[derive(Default, Debug)]
pub struct RunConfig {
    interpreter: Option<String>,
    prescript: Option<Vec<String>>,
    run: String,
    postscript: Option<Vec<String>>
}

pub fn parse_run(conf: &Config) -> Result<RunConfig, ConfigError> {
    let mut run_conf: RunConfig = RunConfig::default();
    let hash_run = conf.section.get("run");
    let run = hash_run.unwrap();

    if !run.contains_key("run") 
        || 
        ((run.contains_key("prescript") || run.contains_key("postscript")) && !run.contains_key("interpreter"))
    {
        return Err(ConfigError::InvalidConfig { message: "Missing run section or 'postscript' and/or 'prescript' provided without interpreter specified".into()  })
    }


    for (k, v) in run.iter() {
        match k.as_str()  {
            "interpreter" => { 
                if v.split_once(" ").is_some() {
                    return Err(ConfigError::InvalidConfig { message: "Only one interpreter is supported at time".into() })
                }
                run_conf.interpreter = Some(v.to_string());
             },
            "prescript" => {
                run_conf.prescript.get_or_insert_with(Vec::new).extend(v.split_whitespace().map(|s| s.to_string()));
            },
            "run" => { 
                if v.split_once(" ").is_some() {
                    return Err(ConfigError::InvalidConfig { message: "Only one target is supported at time".into() })
                }
                run_conf.run = v.to_string() 
            },
            "postscript" => {
                run_conf.postscript.get_or_insert_with(Vec::new).extend(v.split_whitespace().map(|s| s.to_string()));
            },
            _ => {
                eprintln!("Warning: Unknow keyword {k}");
            }
        }
    }

    Ok(run_conf)
} 

pub fn execute_run(run_conf: &RunConfig) -> Result<Vec<CmdOutput>, ConfigError> {
    let mut ret: Vec<CmdOutput> = Vec::new();

    if let Some(interpreter) = &run_conf.interpreter {
        if let Some(prescript) = &run_conf.prescript {
            let tmp = std::process::Command::new(interpreter.to_string()).args(prescript.iter())
                .output().map_err(|_| ConfigError::CommandFailed { cmd: interpreter.clone(), message: "Unexpected".into() })?;
            let tmpoutput: CmdOutput = CmdOutput { 
                stdout: String::from_utf8_lossy(&tmp.stdout).to_string(), 
                stderr: String::from_utf8_lossy(&tmp.stderr).to_string(), 
                status: tmp.status 
            };
            ret.push(tmpoutput);
        }
    }

    let binary = if run_conf.run.starts_with(['/', '\\'])
        || run_conf.run.contains(":\\")
        || run_conf.run.starts_with("./")
        || run_conf.run.starts_with("../") 
    {
        run_conf.run.clone()
    } else {
        if cfg!(windows) {
            format!(".\\{}", run_conf.run)
        } else {
            format!("./{}", run_conf.run)
        }
    };

    let cmd = std::process::Command::new(binary)
        .output().map_err(|_| ConfigError::CommandFailed { cmd: run_conf.run.clone(), message: "Failed to run target".into() })?;
    let cmdoutput: CmdOutput = CmdOutput { 
        stdout: String::from_utf8_lossy(&cmd.stdout).to_string(),
        stderr: String::from_utf8_lossy(&cmd.stderr).to_string(),
        status: cmd.status 
    };

    ret.push(cmdoutput);

    if let Some(interpreter) = &run_conf.interpreter {
        if let Some(postscript) = &run_conf.postscript {
            let tmp = std::process::Command::new(interpreter.to_string()).args(postscript.iter())
                .output().map_err(|_| ConfigError::CommandFailed { cmd: interpreter.clone(), message: "Unexpected".into() })?;
            let tmpoutput: CmdOutput = CmdOutput { 
                stdout: String::from_utf8_lossy(&tmp.stdout).to_string(), 
                stderr: String::from_utf8_lossy(&tmp.stderr).to_string(), 
                status: tmp.status 
            };
            ret.push(tmpoutput);
        }
    }

    Ok(ret)
}