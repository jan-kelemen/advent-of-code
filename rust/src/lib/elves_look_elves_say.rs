use itertools::Itertools;
use std::fs::File;
use std::io::Read;

pub fn part1(mut input: File) -> String {
    let mut sequence = String::new();
    let _ = input.read_to_string(&mut sequence);

    let mut plain = sequence.trim().to_string();
    for _ in 0..40 {
        plain = plain
            .chars()
            .dedup_with_count()
            .fold(String::new(), |mut acc, value| {
                acc.push_str(&value.0.to_string());
                acc.push(value.1);
                acc
            });
    }
    plain.len().to_string()
}

pub fn part2(mut input: File) -> String {
    let mut sequence = String::new();
    let _ = input.read_to_string(&mut sequence);

    let mut plain = sequence.trim().to_string();
    for _ in 0..50 {
        plain = plain
            .chars()
            .dedup_with_count()
            .fold(String::new(), |mut acc, value| {
                acc.push_str(&value.0.to_string());
                acc.push(value.1);
                acc
            });
    }
    plain.len().to_string()
}
