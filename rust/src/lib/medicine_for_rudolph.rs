use itertools::Itertools;
use regex::Regex;
use std::collections::{HashMap, HashSet};
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::sync::LazyLock;

fn apply_substitutions(str: &str, replacements: &HashMap<String, Vec<String>>) -> HashSet<String> {
    let mut rv = HashSet::new();

    let mut skip_count = 0;
    for i in 0..str.len() {
        if skip_count != 0 {
            skip_count -= 1;
            continue;
        }

        for replacement in replacements {
            if str[i..].starts_with(replacement.0.as_str()) {
                let prefix: String = str.chars().take(i).collect();
                let suffix: String = str.chars().dropping(i + replacement.0.len()).collect();
                for replacement_value in replacement.1 {
                    rv.insert(format!("{}{}{}", prefix, replacement_value, suffix));
                }

                skip_count = (*replacement.0).len() - 1;
            }
        }
    }

    rv
}

fn read_replacements(input: File) -> (String, HashMap<String, Vec<String>>) {
    static PARSE: LazyLock<Regex> = LazyLock::new(|| Regex::new(r"(\w+) => (\w+)").unwrap());

    let mut replacements: HashMap<String, Vec<String>> = HashMap::new();
    let mut molecule = String::new();

    let reader = BufReader::new(input);

    reader.lines().for_each(|l| {
        let line = l.unwrap();

        if let Some(captures) = (*PARSE).captures(&line) {
            replacements
                .entry(captures[1].to_string())
                .or_default()
                .push(captures[2].to_string());
        } else if !line.is_empty() {
            molecule = line;
        }
    });

    (molecule, replacements)
}

pub fn part1(input: File) -> String {
    let (medicine, replacements) = read_replacements(input);

    let unique = apply_substitutions(medicine.as_str(), &replacements);

    unique.len().to_string()
}

pub fn part2(input: File) -> String {
    //    let (medicine, replacements) = read_replacements(input);

    //    let steps = 0;
    //    let mut molecules: HashSet<String> = HashSet::from(["e".to_string()]);
    //    while !molecules.contains(&medicine) {
    //        let mut next : HashSet<String> = HashSet::new();
    //        for molecule in molecules.iter() {
    //            next.extend(recurse(molecule, &replacements));
    //        }
    //        steps += 1;
    //        molecules = next;
    //    }

    "".to_string()
}
