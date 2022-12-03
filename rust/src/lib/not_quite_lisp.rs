use std::fs::File;
use std::io::Read;

pub fn part1(mut input: File) -> String {
    let mut instructions = String::new();
    let _size = match input.read_to_string(&mut instructions) {
        Ok(size) => size,
        Err(error) => panic!("Problem reading input file: {:?}", error),
    };

    let mut final_floor = 0;
    for c in instructions.chars() {
        final_floor += match c {
            '(' => 1,
            ')' => -1,
            other => panic!("Unexpected floor instruction: {}", other),
        }
    }

    final_floor.to_string()
}

pub fn part2(mut input: File) -> String {
    let mut instructions = String::new();
    let _size = match input.read_to_string(&mut instructions) {
        Ok(size) => size,
        Err(error) => panic!("Problem reading input file: {:?}", error),
    };

    let mut basement_entered = 0;
    let mut final_floor = 0;
    for (i, c) in instructions.chars().enumerate() {
        final_floor += match c {
            '(' => 1,
            ')' => -1,
            other => panic!("Unexpected floor instruction: {}", other),
        };

        if final_floor < 0 {
            basement_entered = i + 1;
            break;
        }
    }

    basement_entered.to_string()
}
