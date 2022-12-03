use aoclib::*;
use std::env;
use std::fs::File;

fn main() {
    let args: Vec<String> = env::args().collect();
    let (p1, p2) = match args[1].parse::<u8>() {
        Ok(n) => match n {
            1 => (
                not_quite_lisp::final_floor(File::open("res/2015/d01.txt").unwrap()),
                not_quite_lisp::basement_entered(File::open("res/2015/d01.txt").unwrap()),
            ),
            2 => (
                i_was_told_there_would_be_no_math::needed_wrapping_paper_area(
                    File::open("res/2015/d02.txt").unwrap(),
                ),
                i_was_told_there_would_be_no_math::needed_feet_of_ribbon(
                    File::open("res/2015/d02.txt").unwrap(),
                ),
            ),
            e => panic!("Unkown day: {}", e),
        },
        Err(e) => panic!("Unknown day: {}", e),
    };

    print!("{} {}", p1, p2);
}
