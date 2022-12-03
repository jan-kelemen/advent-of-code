use std::fs::File;
use std::io::Read;

fn with_leading_zeroes(key: &str, count: usize) -> u64 {
    let pattern = "0".repeat(count);

    let mut number: u64 = 0;
    loop {
        let result = format!("{:x}", md5::compute(format!("{}{}", key, number)));

        if result.starts_with(&pattern) {
            break number;
        }
        number += 1;
    }
}

pub fn part1(mut input: File) -> String {
    let mut key = String::new();
    if let Err(e) = input.read_to_string(&mut key) {
        panic!("Problem reading input file: {}", e)
    }

    with_leading_zeroes(key.as_str(), 5).to_string()
}

pub fn part2(mut input: File) -> String {
    let mut key = String::new();
    if let Err(e) = input.read_to_string(&mut key) {
        panic!("Problem reading input file: {}", e)
    }

    with_leading_zeroes(key.as_str(), 6).to_string()
}
