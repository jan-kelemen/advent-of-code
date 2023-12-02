use std::collections::HashMap;
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::str::FromStr;

use itertools::Itertools;
use lazy_static::lazy_static;
use regex::Regex;

fn construct_graph(input: File) -> HashMap<String, HashMap<String, u64>> {
    lazy_static! {
        static ref PARSE: Regex = Regex::new(r"([[:alpha:]]+) to ([[:alpha:]]+) = (\d+)").unwrap();
    }

    let mut nodes = HashMap::<String, HashMap<String, u64>>::new();

    let reader = BufReader::new(input);

    reader.lines().for_each(|l| {
        let line = l.unwrap();
        let captures = PARSE.captures(&line).unwrap();

        let from = captures[1].to_string();
        let to = captures[2].to_string();
        let weight = u64::from_str(&captures[3]).unwrap();

        nodes
            .entry(from.clone())
            .or_default()
            .entry(to.clone())
            .or_insert(weight);
        nodes.entry(to).or_default().entry(from).or_insert(weight);
    });

    nodes
}

fn all_paths(graph: HashMap<String, HashMap<String, u64>>) -> Vec<u64> {
    graph
        .keys()
        .permutations(graph.keys().len())
        .map(|permutation| {
            let mut weight = 0;

            for flight in permutation.windows(2) {
                let source = flight[0];
                let target = flight[1];
                weight += graph[source][target];
            }
            weight
        })
        .collect()
}

pub fn part1(input: File) -> String {
    let graph = construct_graph(input);

    all_paths(graph).iter().min().unwrap().to_string()
}

pub fn part2(input: File) -> String {
    let graph = construct_graph(input);

    all_paths(graph).iter().max().unwrap().to_string()
}
