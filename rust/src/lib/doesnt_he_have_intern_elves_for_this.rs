use fancy_regex::Regex;
use lazy_static::lazy_static;
use std::fs::File;
use std::io::{BufRead, BufReader};

fn old_rules(string: &String) -> bool {
    lazy_static! {
        static ref BAD_SUBSTR: Regex = Regex::new("ab|cd|pq|xy").unwrap();
        static ref CONSECUTIVE_LETTERS: Regex = Regex::new(r"([[:alpha:]])\1").unwrap();
    }

    let atleast_three_vowels = string.matches(&['a', 'e', 'i', 'o', 'u']).count() >= 3;
    let consecutive_letters = CONSECUTIVE_LETTERS.is_match(string).unwrap();
    let no_bad_substrs = !BAD_SUBSTR.is_match(string).unwrap();

    atleast_three_vowels && consecutive_letters && no_bad_substrs
}

fn new_rules(string: &String) -> bool {
    lazy_static! {
        static ref PAIR_WITHOUT_OVERLAP: Regex =
            Regex::new(r"([[:alpha:]]{2})[[:alpha:]]*\1").unwrap();
        static ref REPEATED: Regex = Regex::new(r"([[:alpha:]])[[:alpha:]]\1").unwrap();
    }

    let pair_without_overlap = PAIR_WITHOUT_OVERLAP.is_match(string).unwrap();
    let repeated_letter = REPEATED.is_match(string).unwrap();

    pair_without_overlap && repeated_letter
}

pub fn part1(input: File) -> String {
    let reader = BufReader::new(input);

    reader
        .lines()
        .fold(0, |acc, line| acc + old_rules(&line.unwrap()) as i32)
        .to_string()
}

pub fn part2(input: File) -> String {
    let reader = BufReader::new(input);

    reader
        .lines()
        .fold(0, |acc, line| acc + new_rules(&line.unwrap()) as i32)
        .to_string()
}

#[test]
fn examples_of_old_rules() {
    assert!(old_rules(&"ugknbfddgicrmopn".to_string()));
    assert!(old_rules(&"aaa".to_string()));
    assert!(!old_rules(&"jchzalrnumimnmhp".to_string()));
    assert!(!old_rules(&"haegwjzuvuyypxyu".to_string()));
    assert!(!old_rules(&"dvszwmarrgswjxmb".to_string()));
}

#[test]
fn examples_of_new_rules() {
    assert!(new_rules(&"qjhvhtzxzqqjkmpb".to_string()));
    assert!(new_rules(&"xxyxx".to_string()));
    assert!(!new_rules(&"uurcxstgmygtbstg".to_string()));
    assert!(!new_rules(&"ieodomkazucvgmuy".to_string()));
}
