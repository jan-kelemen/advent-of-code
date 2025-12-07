use std::collections::HashMap;
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::str::FromStr;
use std::sync::LazyLock;

use regex::Regex;

fn read_compounds(input: File) -> Vec<HashMap<String, u32>> {
    static PARSE: LazyLock<Regex> =
        LazyLock::new(|| Regex::new(r"Sue \d+: (\w+): (\d+), (\w+): (\d+), (\w+): (\d+)").unwrap());

    let mut compounds = Vec::new();

    let reader = BufReader::new(input);

    reader.lines().for_each(|l| {
        let line = l.unwrap();
        let captures = (*PARSE).captures(&line).unwrap();

        compounds.push(HashMap::from([
            (
                captures[1].to_string(),
                u32::from_str(&captures[2]).unwrap_or_default(),
            ),
            (
                captures[3].to_string(),
                u32::from_str(&captures[4]).unwrap_or_default(),
            ),
            (
                captures[5].to_string(),
                u32::from_str(&captures[6]).unwrap_or_default(),
            ),
        ]));
    });

    compounds
}

pub fn part1(input: File) -> String {
    let aunts = read_compounds(input);

    let query = HashMap::from([
        ("children", 3),
        ("cats", 7),
        ("samoyeds", 2),
        ("pomeranians", 3),
        ("akitas", 0),
        ("vizslas", 0),
        ("goldfish", 5),
        ("trees", 3),
        ("cars", 2),
        ("perfumes", 1),
    ]);

    let mut num = 0;
    for (number, sue) in aunts.iter().enumerate() {
        let mut total = 0;
        for (name, count) in sue.iter() {
            if let Some(query_count) = query.get(name.as_str()) {
                if query_count == count {
                    total += 1;
                }
            }
        }

        if total == sue.len() {
            num = number + 1;
            break;
        }
    }

    num.to_string()
}

pub fn part2(input: File) -> String {
    let aunts = read_compounds(input);

    let query = HashMap::from([
        ("children", 3),
        ("cats", 7),
        ("samoyeds", 2),
        ("pomeranians", 3),
        ("akitas", 0),
        ("vizslas", 0),
        ("goldfish", 5),
        ("trees", 3),
        ("cars", 2),
        ("perfumes", 1),
    ]);

    let mut num = 0;
    for (number, sue) in aunts.iter().enumerate() {
        let mut total = 0;
        for (name, count) in sue.iter() {
            if let Some(query_count) = query.get(name.as_str()) {
                let test = match name.as_str() {
                    "cats" => count > query_count,
                    "trees" => count > query_count,
                    "pomeranians" => count < query_count,
                    "goldfish" => count < query_count,
                    _ => count == query_count,
                };

                if test {
                    total += 1;
                }
            }
        }

        if total == sue.len() {
            num = number + 1;
            break;
        }
    }

    num.to_string()
}
