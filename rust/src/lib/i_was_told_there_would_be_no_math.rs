use lazy_static::lazy_static;
use regex::Regex;
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::str::FromStr;

#[derive(Debug)]
struct Box {
    length: u32,
    width: u32,
    height: u32,
}

impl FromStr for Box {
    type Err = std::num::ParseIntError;

    // instance of 'RGB'
    fn from_str(dimensions: &str) -> Result<Self, Self::Err> {
        lazy_static! {
            static ref RE: Regex = Regex::new(r"(\d+)x(\d+)x(\d+)").unwrap();
        }

        let captures = RE.captures(dimensions).unwrap();
        let length = u32::from_str(&captures[1]).unwrap();
        let width = u32::from_str(&captures[2]).unwrap();
        let height = u32::from_str(&captures[3]).unwrap();

        Ok(Box {
            length,
            width,
            height,
        })
    }
}

fn wrapping_paper_area(b: Box) -> u32 {
    let sides = [b.length * b.width, b.width * b.height, b.height * b.length];

    2 * sides.iter().sum::<u32>() + sides.iter().min().unwrap()
}

fn ribbon_length(b: Box) -> u32 {
    let side_perimiters = [b.length + b.width, b.width + b.height, b.height + b.length];
    let volume = b.length * b.width * b.height;

    2 * side_perimiters.iter().min().unwrap() + volume
}

pub fn part1(input: File) -> String {
    let reader = BufReader::new(input);

    let mut total_area = 0;
    for line in reader.lines() {
        total_area += match line {
            Ok(line) => wrapping_paper_area(Box::from_str(line.as_str()).unwrap()),
            Err(e) => panic!("Error while reading input: {}", e),
        }
    }

    total_area.to_string()
}

pub fn part2(input: File) -> String {
    let reader = BufReader::new(input);

    let mut total_ribbon_length = 0;
    for line in reader.lines() {
        total_ribbon_length += match line {
            Ok(line) => ribbon_length(Box::from_str(line.as_str()).unwrap()),
            Err(e) => panic!("Error while reading input: {}", e),
        }
    }

    total_ribbon_length.to_string()
}
