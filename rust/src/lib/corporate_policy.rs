use itertools::Itertools;
use std::collections::HashSet;
use std::fs::File;
use std::io::Read;

fn test_password(password: &String) -> bool {
    let mut increasing = false;
    for (a, b, c) in password.chars().map(|c| u32::from(c)).tuple_windows() {
        if b > a && c > b && b - a == 1 && c - b == 1 {
            increasing = true
        }
    }

    let ambiguous = password.matches(&['i', 'o', 'l']).count() == 0;

    let mut sets = HashSet::new();
    for (a, b) in password.chars().tuple_windows() {
        if a == b {
            sets.insert(a);
        }
    }

    increasing && ambiguous && sets.len() >= 2
}

fn next_password(password: &String) -> String {
    let mut rv = String::new();

    let mut do_replace = true;
    for i in 0usize..8usize {
        if let Some(c) = password.chars().nth_back(i) {
            if do_replace {
                if c != 'z' {
                    if let Some(next) = char::from_u32(u32::from(c) + 1) {
                        rv.insert(0, next)
                    }
                    do_replace = false;
                    continue;
                }
                rv.insert(0, 'a')
            } else {
                rv.insert(0, c)
            }
        }
    }

    rv
}

pub fn part1(mut input: File) -> String {
    let mut password = String::new();

    match input.read_to_string(&mut password) {
        Ok(size) => size,
        Err(error) => panic!("Problem reading input file: {:?}", error),
    };

    while !test_password(&password) {
        password = next_password(&password);
    }

    password
}

pub fn part2(input: File) -> String {
    let mut password = next_password(&part1(input));

    while !test_password(&password) {
        password = next_password(&password);
    }

    password
}

#[test]
fn password_requirements() {
    assert!(!test_password(&"hijklmmn".to_string()));
    assert!(!test_password(&"abbceffg".to_string()));
    assert!(!test_password(&"abbcegjk".to_string()));
    assert!(test_password(&"abcdffaa".to_string()));
    assert!(test_password(&"ghjaabcc".to_string()));
}

#[test]
fn increment_password() {
    assert_eq!(next_password(&"hijklmmn".to_string()), "hijklmmo");
    assert_eq!(next_password(&"hijklmmz".to_string()), "hijklmna");
}
