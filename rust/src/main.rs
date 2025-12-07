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
            6 => (
                probably_a_fire_hazard::part1(input_file(n)),
                probably_a_fire_hazard::part2(input_file(n)),
            ),
            7 => (
                some_assembly_required::part1(input_file(n)),
                some_assembly_required::part2(input_file(n)),
            ),
            8 => (
                matchsticks::part1(input_file(n)),
                matchsticks::part2(input_file(n)),
            ),
            9 => (
                all_in_a_single_night::part1(input_file(n)),
                all_in_a_single_night::part2(input_file(n)),
            ),
            10 => (
                elves_look_elves_say::part1(input_file(n)),
                elves_look_elves_say::part2(input_file(n)),
            ),
            11 => (
                corporate_policy::part1(input_file(n)),
                corporate_policy::part2(input_file(n)),
            ),
            12 => (
                jsabacusframework_io::part1(input_file(n)),
                jsabacusframework_io::part2(input_file(n)),
            ),
            13 => (
                knights_of_the_dinner_table::part1(input_file(n)),
                knights_of_the_dinner_table::part2(input_file(n)),
            ),
            14 => (
                raindeer_olympics::part1(input_file(n)),
                raindeer_olympics::part2(input_file(n)),
            ),
            15 => (
                science_for_hungry_people::part1(input_file(n)),
                science_for_hungry_people::part2(input_file(n)),
            ),
            16 => (
                aunt_sue::part1(input_file(n)),
                aunt_sue::part2(input_file(n)),
            ),
            e => panic!("Unknown day: {}", e),
        },
        Err(e) => panic!("Unknown day: {}", e),
    };

    println!("{} {}", p1, p2);
}
