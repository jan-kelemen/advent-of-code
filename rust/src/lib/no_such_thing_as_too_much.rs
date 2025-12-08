use std::cmp::max;
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::str::FromStr;

use itertools::Itertools;

fn read_containers(input: File) -> Vec<u32> {
    let reader = BufReader::new(input);
    reader
        .lines()
        .map(|l| u32::from_str(l.unwrap().as_str()).unwrap())
        .collect()
}

pub fn part1(input: File) -> String {
    let containers = read_containers(input);

    let mut count = 0;
    for i in 1..containers.len() {
        for comb in containers.iter().combinations(i) {
            let sum: u32 = comb.into_iter().sum();
            if sum == 150 {
                count += 1;
            }
        }
    }

    count.to_string()
}

pub fn part2(input: File) -> String {
    let containers = read_containers(input);

    let mut count: Vec<Vec<u32>> = Vec::new();

    for i in 1..containers.len() {
        for comb in containers.iter().combinations(i) {
            let sum: u32 = comb.iter().map(|&s| *s).sum();
            if sum == 150 {
                let fits: Vec<u32> = comb.into_iter().map(|s| *s).collect();
                count.push(fits);
            }
        }

        if !count.is_empty() {
            break;
        }
    }

    count.len().to_string()
}
