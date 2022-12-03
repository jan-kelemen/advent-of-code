use std::collections::HashSet;
use std::fs::File;
use std::io::Read;

#[derive(Clone, Hash, Eq, PartialEq, Debug)]
struct Coordinate {
    x: i64,
    y: i64,
}

impl Coordinate {
    fn new() -> Coordinate {
        Coordinate { x: 0, y: 0 }
    }

    fn move_in_direction(&self, direction: char) -> Coordinate {
        match direction {
            '^' => Coordinate {
                x: self.x,
                y: self.y + 1,
            },
            'v' => Coordinate {
                x: self.x,
                y: self.y - 1,
            },
            '>' => Coordinate {
                x: self.x + 1,
                y: self.y,
            },
            '<' => Coordinate {
                x: self.x - 1,
                y: self.y,
            },
            d => panic!("Unexpected direction: {}", d),
        }
    }
}

pub fn part1(mut input: File) -> String {
    let mut instructions = String::new();
    if let Err(e) = input.read_to_string(&mut instructions) {
        panic!("Problem reading input file: {}", e);
    }

    let mut current_coordinate = Coordinate::new();
    let mut coordinates = HashSet::from([current_coordinate.clone()]);
    for instruction in instructions.chars() {
        current_coordinate = current_coordinate.move_in_direction(instruction);

        coordinates.insert(current_coordinate.clone());
    }

    coordinates.len().to_string()
}

pub fn part2(mut input: File) -> String {
    let mut instructions = String::new();
    if let Err(e) = input.read_to_string(&mut instructions) {
        panic!("Problem reading input file: {}", e);
    }

    let mut santas_turn = true;
    let mut santa_coordinate = Coordinate::new();
    let mut robo_santa_coordinate = Coordinate::new();

    let mut coordinates = HashSet::from([santa_coordinate.clone()]);
    for instruction in instructions.chars() {
        if santas_turn {
            santa_coordinate = santa_coordinate.move_in_direction(instruction);
            coordinates.insert(santa_coordinate.clone());
        } else {
            robo_santa_coordinate = robo_santa_coordinate.move_in_direction(instruction);
            coordinates.insert(robo_santa_coordinate.clone());
        }

        santas_turn ^= true;
    }

    coordinates.len().to_string()
}
