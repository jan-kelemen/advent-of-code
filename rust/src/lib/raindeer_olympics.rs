use std::cmp::min;
use std::collections::HashMap;
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::str::FromStr;
use std::sync::LazyLock;

use regex::Regex;

#[derive(Debug)]
struct Raindeer {
    speed: u32,
    fly_time: u32,
    rest_time: u32,
}

fn read_statistics(input: File) -> Vec<Raindeer> {
    static PARSE: LazyLock<Regex> = LazyLock::new(|| {
        Regex::new(
            r"\w+ can fly (\d+) km/s for (\d+) seconds, but then must rest for (\d+) seconds.",
        )
        .unwrap()
    });

    let mut raindeer = Vec::new();

    let reader = BufReader::new(input);

    reader.lines().for_each(|l| {
        let line = l.unwrap();
        let captures = (*PARSE).captures(&line).unwrap();

        raindeer.push(Raindeer {
            speed: u32::from_str(&captures[1]).unwrap_or_default(),
            fly_time: u32::from_str(&captures[2]).unwrap_or_default(),
            rest_time: u32::from_str(&captures[3]).unwrap_or_default(),
        });
    });

    raindeer
}

fn distance_at_time(deer: &Raindeer, time: u32) -> u32 {
    let period = deer.fly_time + deer.rest_time;
    let remaining = min(time % period, deer.fly_time);

    (time / period) * deer.fly_time * deer.speed + remaining * deer.speed
}

pub fn part1(input: File) -> String {
    let raindeer = read_statistics(input);

    let time = 2503;
    let distance = raindeer
        .iter()
        .map(|current| distance_at_time(current, time))
        .max()
        .unwrap();

    distance.to_string()
}

pub fn part2(input: File) -> String {
    let raindeer = read_statistics(input);

    let mut points: HashMap<usize, u32> = HashMap::new();

    let time = 2504;
    for i in 1..time {
        if let Some(max_distance) = raindeer.iter().map(|d| distance_at_time(d, i)).max() {
            // If there are multiple reindeer tied for the lead, they each get one point.
            for (i, distance) in raindeer
                .iter()
                .enumerate()
                .map(|x| (x.0, distance_at_time(x.1, i)))
            {
                if distance == max_distance {
                    if let Some(point) = points.get(&i) {
                        points.insert(i, point + 1);
                    } else {
                        points.insert(i, 1);
                    }
                }
            }
        }
    }

    points.values().max().unwrap().to_string()
}

#[test]
fn test_distance_at_time() {
    let comet = Raindeer {
        speed: 14,
        fly_time: 10,
        rest_time: 127,
    };
    assert_eq!(distance_at_time(&comet, 1), 14);
    assert_eq!(distance_at_time(&comet, 10), 140);
    assert_eq!(distance_at_time(&comet, 11), 140);
    assert_eq!(distance_at_time(&comet, 1000), 1120);
    let dancer = Raindeer {
        speed: 16,
        fly_time: 11,
        rest_time: 162,
    };
    assert_eq!(distance_at_time(&dancer, 1), 16);
    assert_eq!(distance_at_time(&dancer, 10), 160);
    assert_eq!(distance_at_time(&dancer, 11), 176);
    assert_eq!(distance_at_time(&dancer, 1000), 1056);
}
