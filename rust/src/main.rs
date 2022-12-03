use aoclib::*;
use std::env;
use std::fs::File;

fn main() {
    let args: Vec<String> = env::args().collect();
    let (p1, p2) = match args[1].parse::<u8>() {
        Ok(n) => match n {
            1 => (
                not_quite_lisp::part1(File::open("res/2015/d01.txt").unwrap()),
                not_quite_lisp::part2(File::open("res/2015/d01.txt").unwrap()),
            ),
            2 => (
                i_was_told_there_would_be_no_math::part1(File::open("res/2015/d02.txt").unwrap()),
                i_was_told_there_would_be_no_math::part2(File::open("res/2015/d02.txt").unwrap()),
            ),
            3 => (
                perfectly_spherical_houses_in_a_vacuum::part1(
                    File::open("res/2015/d03.txt").unwrap(),
                ),
                perfectly_spherical_houses_in_a_vacuum::part2(
                    File::open("res/2015/d03.txt").unwrap(),
                ),
            ),
            e => panic!("Unkown day: {}", e),
        },
        Err(e) => panic!("Unknown day: {}", e),
    };

    println!("{} {}", p1, p2);
}
