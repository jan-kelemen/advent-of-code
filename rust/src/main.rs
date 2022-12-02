use aoclib;
use std::env;
use std::fs::File;

fn main() {
    let args: Vec<String> = env::args().collect();
    let (p1, p2) = match args[1].parse::<u8>() {
        Ok(n) => match n {
            1 => (
                aoclib::d01p01(File::open("res/2015/d01.txt").unwrap()),
                aoclib::d01p02(File::open("res/2015/d01.txt").unwrap()),
            ),
            2 => (
                aoclib::d02p01(File::open("res/2015/d02.txt").unwrap()),
                aoclib::d02p02(File::open("res/2015/d02.txt").unwrap()),
            ),
            e => panic!("Unkown day: {}", e),
        },
        Err(e) => panic!("Unknown day: {}", e),
    };

    print!("{} {}", p1, p2);
}
