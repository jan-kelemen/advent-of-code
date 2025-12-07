use std::collections::HashMap;
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::str::FromStr;
use std::sync::LazyLock;

use itertools::Itertools;
use regex::Regex;

fn read_sitting(input: File) -> HashMap<String, HashMap<String, i32>> {
    static PARSE: LazyLock<Regex> = LazyLock::new(|| {
        Regex::new(r"(\w+) would (\w+) (\d+) happiness units by sitting next to (\w+).").unwrap()
    });

    let mut guests = HashMap::<String, HashMap<String, i32>>::new();

    let reader = BufReader::new(input);

    reader.lines().for_each(|l| {
        let line = l.unwrap();
        let captures = (*PARSE).captures(&line).unwrap();

        let first = captures[1].to_string();
        let would_lose = captures[2].contains("lose");
        let second = captures[4].to_string();

        if let Ok(points) = i32::from_str(&captures[3].to_string()) {
            guests
                .entry(first)
                .or_default()
                .insert(second, if would_lose { points * -1 } else { points });
        }
    });

    guests
}

fn all_paths(guests: &HashMap<String, HashMap<String, i32>>) -> Vec<i32> {
    guests
        .keys()
        .permutations(guests.keys().len())
        .map(|permutation| {
            let mut happiness = 0;

            for (left, center, right) in permutation.iter().circular_tuple_windows() {
                if let Some(guest) = guests.get(*center) {
                    if let Some(on_left) = guest.get(*left) {
                        happiness += on_left
                    }

                    if let Some(on_right) = guest.get(*right) {
                        happiness += on_right
                    }
                }
            }
            happiness
        })
        .collect()
}

pub fn part1(input: File) -> String {
    let guests = read_sitting(input);

    all_paths(&guests).into_iter().max().unwrap().to_string()
}

pub fn part2(input: File) -> String {
    let mut guests = read_sitting(input);
    guests.insert("Me".to_string(), HashMap::new());

    all_paths(&guests).into_iter().max().unwrap().to_string()
}
