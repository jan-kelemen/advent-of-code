use std::cmp::max;
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::str::FromStr;
use std::sync::LazyLock;

use itertools::Itertools;
use regex::Regex;

struct Stats {
    capacity: i32,
    durability: i32,
    flavor: i32,
    texture: i32,
    calories: i32,
}

fn read_ingredients(input: File) -> Vec<Stats> {
    static PARSE: LazyLock<Regex> = LazyLock::new(|| {
        Regex::new(r"\w+: capacity (-?\d+), durability (-?\d+), flavor (-?\d+), texture (-?\d+), calories (-?\d+)").unwrap()
    });

    let mut ingredients = Vec::new();

    let reader = BufReader::new(input);

    reader.lines().for_each(|l| {
        let line = l.unwrap();
        let captures = (*PARSE).captures(&line).unwrap();

        ingredients.push(Stats {
            capacity: i32::from_str(&captures[1]).unwrap_or_default(),
            durability: i32::from_str(&captures[2]).unwrap_or_default(),
            flavor: i32::from_str(&captures[3]).unwrap_or_default(),
            texture: i32::from_str(&captures[4]).unwrap_or_default(),
            calories: i32::from_str(&captures[5]).unwrap_or_default(),
        });
    });

    ingredients
}

fn ingredient_sum(combinations: &Vec<u32>) -> u32 {
    combinations.iter().sum()
}

fn evaluate_score(
    ingredients: &Vec<Stats>,
    amounts: &Vec<&u32>,
    calorie_limit: Option<i32>,
) -> u32 {
    let mut categorized = Stats {
        capacity: 0,
        durability: 0,
        flavor: 0,
        texture: 0,
        calories: 0,
    };

    for (i, amount) in amounts.iter().enumerate() {
        if let Some(ingredient) = ingredients.get(i) {
            categorized.capacity += (**amount as i32) * ingredient.capacity;
            categorized.durability += (**amount as i32) * ingredient.durability;
            categorized.flavor += (**amount as i32) * ingredient.flavor;
            categorized.texture += (**amount as i32) * ingredient.texture;
            categorized.calories += (**amount as i32) * ingredient.calories;
        }
    }

    if let Some(limit) = calorie_limit {
        if categorized.calories != limit {
            return 0;
        }
    }

    if categorized.capacity < 0
        || categorized.durability < 0
        || categorized.flavor < 0
        || categorized.texture < 0
    {
        return 0;
    }

    max(
        categorized.capacity * categorized.durability * categorized.flavor * categorized.texture,
        0,
    ) as u32
}
fn ingredient_combinations(bound: u32, length: usize) -> Vec<Vec<u32>> {
    let range = 0..(bound + 1);

    let mut rv = Vec::new();
    for x in range.combinations_with_replacement(length) {
        if ingredient_sum(&x) == bound {
            rv.push(x)
        }
    }

    rv
}

pub fn part1(input: File) -> String {
    let ingredients = read_ingredients(input);

    let mut max_score = 0;
    for combinations in ingredient_combinations(100, ingredients.len()) {
        for permutation in combinations.iter().permutations(ingredients.len()) {
            max_score = max(max_score, evaluate_score(&ingredients, &permutation, None))
        }
    }

    max_score.to_string()
}

pub fn part2(input: File) -> String {
    let ingredients = read_ingredients(input);

    let mut max_score = 0;
    for combinations in ingredient_combinations(100, ingredients.len()) {
        for permutation in combinations.iter().permutations(ingredients.len()) {
            max_score = max(
                max_score,
                evaluate_score(&ingredients, &permutation, Some(500)),
            )
        }
    }

    max_score.to_string()
}
