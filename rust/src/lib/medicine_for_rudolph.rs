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

#[derive(PartialEq)]
enum Symbol {
    Al,
    Ar,
    B,
    C,
    Ca,
    e,
    F,
    H,
    Mg,
    N,
    O,
    P,
    Rn,
    Si,
    Th,
    Ti,
    Y,
}

fn convert_medicine_to_symbols(medicine: &String) -> Vec<Symbol> {
    let mut rv = Vec::new();

    for (i, str) in medicine.match_indices(|c: char| c.is_uppercase() || c == 'e') {
        match str {
            "A" => match medicine.chars().nth(i + 1) {
                Some('l') => rv.push(Symbol::Al),
                Some('r') => rv.push(Symbol::Ar),
                _ => panic!("Unrecognized symbol A"),
            },
            "B" => rv.push(Symbol::B),
            "C" => match medicine.chars().nth(i + 1) {
                Some('a') => rv.push(Symbol::Ca),
                _ => rv.push(Symbol::C),
            },
            "e" => rv.push(Symbol::e),
            "F" => rv.push(Symbol::F),
            "H" => rv.push(Symbol::H),
            "M" => rv.push(Symbol::Mg),
            "N" => rv.push(Symbol::N),
            "O" => rv.push(Symbol::O),
            "P" => rv.push(Symbol::P),
            "R" => rv.push(Symbol::Rn),
            "S" => rv.push(Symbol::Si),
            "T" => match medicine.chars().nth(i + 1) {
                Some('h') => rv.push(Symbol::Th),
                Some('i') => rv.push(Symbol::Ti),
                _ => panic!("Unrecognized symbol T"),
            },
            "Y" => rv.push(Symbol::Y),
            _ => panic!("Unrecognized symbol {}", str),
        }
    }

    rv
}

pub fn part1(input: File) -> String {
    let (medicine, replacements) = read_replacements(input);
    let unique = apply_substitutions(medicine.as_str(), &replacements);

    unique.len().to_string()
}

pub fn part2(input: File) -> String {
    // If the grammar was in Chomsky Normal Form (CNF) the answer would be 2 * len(medicine) - 1.
    //
    // It should be solvable via CYK algorithm, apparently it works even if the input isn't in CNF.
    // https://en.wikipedia.org/wiki/CYK_algorithm
    // https://stackoverflow.com/questions/36901948/does-cky-really-require-cnf

    let (medicine, _) = read_replacements(input);
    let converted = convert_medicine_to_symbols(&medicine);

    // https://www.reddit.com/r/adventofcode/comments/3xflz8/comment/cy4etju/
    // https://www.reddit.com/r/adventofcode/comments/3xflz8/comment/cy4h7ji/
    let steps = converted.len()
        - converted
            .iter()
            .filter(|s| **s == Symbol::Ar || **s == Symbol::Rn)
            .count()
        - converted.iter().filter(|s| **s == Symbol::Y).count() * 2
        - 1;

    steps.to_string()
}
