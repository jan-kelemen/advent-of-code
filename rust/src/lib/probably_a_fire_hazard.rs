use lazy_static::lazy_static;
use regex::Regex;
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::str::FromStr;

struct Grid {
    lights: [[u32; 1000]; 1000],
}

struct Point {
    x: usize,
    y: usize,
}

impl Grid {
    fn new() -> Grid {
        Grid {
            lights: [[0; 1000]; 1000],
        }
    }

    fn apply(&mut self, lower_left: &Point, upper_right: &Point, f: &dyn Fn(u32) -> u32) {
        for i in lower_left.x..=upper_right.x {
            for j in lower_left.y..=upper_right.y {
                self.lights[i][j] = f(self.lights[i][j])
            }
        }
    }

    fn turned_on(&self) -> u64 {
        self.lights.iter().fold(0, |acc, row| {
            acc + row.iter().fold(0, |acc, light| acc + (*light >= 1) as u64)
        })
    }

    fn total_brightness(&self) -> u64 {
        self.lights.iter().fold(0, |acc, row| {
            acc + row.iter().fold(0, |acc, light| acc + *light as u64)
        })
    }
}

trait Rules {
    fn turn_off(&self) -> &'static dyn Fn(u32) -> u32;
    fn turn_on(&self) -> &'static dyn Fn(u32) -> u32;
    fn toggle(&self) -> &'static dyn Fn(u32) -> u32;
}

struct OldRules {}

impl Rules for OldRules {
    fn turn_off(&self) -> &'static dyn Fn(u32) -> u32 {
        &|_| 0
    }

    fn turn_on(&self) -> &'static dyn Fn(u32) -> u32 {
        &|_| 1
    }

    fn toggle(&self) -> &'static dyn Fn(u32) -> u32 {
        &|l| if l == 0 { 1 } else { 0 }
    }
}

struct NewRules {}

impl Rules for NewRules {
    fn turn_off(&self) -> &'static dyn Fn(u32) -> u32 {
        &|l| if l != 0 { l - 1 } else { 0 }
    }

    fn turn_on(&self) -> &'static dyn Fn(u32) -> u32 {
        &|l| l + 1
    }

    fn toggle(&self) -> &'static dyn Fn(u32) -> u32 {
        &|l| l + 2
    }
}
fn process_instruction(grid: &mut Grid, instruction: &str, rules: &impl Rules) {
    lazy_static! {
        static ref PARSE: Regex =
            Regex::new(r"(turn on|turn off|toggle)+ (\d+),(\d+) through (\d+),(\d+)").unwrap();
    }

    let captures = PARSE.captures(instruction).unwrap();

    let lower_left = Point {
        x: usize::from_str(&captures[2]).unwrap(),
        y: usize::from_str(&captures[3]).unwrap(),
    };
    let upper_right = Point {
        x: usize::from_str(&captures[4]).unwrap(),
        y: usize::from_str(&captures[5]).unwrap(),
    };

    match &captures[1] {
        "turn on" => grid.apply(&lower_left, &upper_right, rules.turn_on()),
        "turn off" => grid.apply(&lower_left, &upper_right, rules.turn_off()),
        "toggle" => grid.apply(&lower_left, &upper_right, rules.toggle()),
        i => panic!("Unexpected command: {}", i),
    };
}

pub fn part1(input: File) -> String {
    let reader = BufReader::new(input);

    let mut grid = Grid::new();

    reader
        .lines()
        .for_each(|line| process_instruction(&mut grid, &line.unwrap(), &OldRules {}));

    grid.turned_on().to_string()
}

pub fn part2(input: File) -> String {
    let reader = BufReader::new(input);

    let mut grid = Grid::new();

    reader
        .lines()
        .for_each(|line| process_instruction(&mut grid, &line.unwrap(), &NewRules {}));

    grid.total_brightness().to_string()
}
