use aoclib::*;
use std::env;
use std::fs::File;

fn input_file(day: u8) -> File {
    File::open(format!("res/2015/d{:02}.txt", day)).unwrap()
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let (p1, p2) = match args[1].parse::<u8>() {
        Ok(n) => match n {
            1 => (
                not_quite_lisp::part1(input_file(n)),
                not_quite_lisp::part2(input_file(n)),
            ),
            2 => (
                i_was_told_there_would_be_no_math::part1(input_file(n)),
                i_was_told_there_would_be_no_math::part2(input_file(n)),
            ),
            3 => (
                perfectly_spherical_houses_in_a_vacuum::part1(input_file(n)),
                perfectly_spherical_houses_in_a_vacuum::part2(input_file(n)),
            ),
            4 => (
                the_ideal_stocking_stuffer::part1(input_file(n)),
                the_ideal_stocking_stuffer::part2(input_file(n)),
            ),
            5 => (
                doesnt_he_have_intern_elves_for_this::part1(input_file(n)),
                doesnt_he_have_intern_elves_for_this::part2(input_file(n)),
            ),
            e => panic!("Unkown day: {}", e),
        },
        Err(e) => panic!("Unknown day: {}", e),
    };

    println!("{} {}", p1, p2);
}
