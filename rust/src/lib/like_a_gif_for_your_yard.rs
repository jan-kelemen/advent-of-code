use itertools::Itertools;
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::mem;

fn read_grid(input: File) -> Vec<Vec<bool>> {
    let reader = BufReader::new(input);
    reader
        .lines()
        .map(|l| {
            l.unwrap()
                .chars()
                .map(|c| c == '#')
                .into_iter()
                .collect_vec()
        })
        .collect()
}

fn neighbor_value(grid: &Vec<Vec<bool>>, x: usize, y: usize) -> bool {
    if let Some(row) = grid.get(y) {
        if let Some(value) = row.get(x) {
            return *value;
        }
    }
    false
}

fn neighbor_on_count(grid: &Vec<Vec<bool>>, x: usize, y: usize) -> u32 {
    let mut count = 0;
    for i in -1..2 {
        for j in -1..2 {
            if i == 0 && j == 0 {
                continue;
            }

            if let (Some(x), Some(y)) = (x.checked_add_signed(i), y.checked_add_signed(j)) {
                if neighbor_value(grid, x, y) {
                    count += 1;
                }
            }
        }
    }

    count
}

fn is_corner(size: usize, x: usize, y: usize) -> bool {
    x == 0 && y == 0 || x == size && y == size || x == 0 && y == size || x == size && y == 0
}

pub fn part1(input: File) -> String {
    let mut grid = read_grid(input);
    let mut new_grid = grid.clone();

    for _ in 0..100 {
        for vec in new_grid.iter_mut() {
            vec.fill(false)
        }

        for i in 0..grid.len() {
            for j in 0..grid.len() {
                let new_value = match neighbor_value(&grid, i, j) {
                    true => match neighbor_on_count(&grid, i, j) {
                        2 | 3 => true,
                        _ => false,
                    },
                    false => neighbor_on_count(&grid, i, j) == 3,
                };

                if let Some(row) = new_grid.get_mut(j) {
                    if let Some(value) = row.get_mut(i) {
                        *value = new_value;
                    }
                }
            }
        }

        mem::swap(&mut new_grid, &mut grid);
    }

    let on: usize = grid
        .iter()
        .map(|row| row.iter().filter(|c| **c).count())
        .sum();
    on.to_string()
}

pub fn part2(input: File) -> String {
    let mut grid = read_grid(input);
    let mut new_grid = grid.clone();

    for i in vec![0, grid.len() - 1] {
        if let Some(row) = grid.get_mut(i) {
            if let Some(c) = row.first_mut() {
                *c = true;
            }
            if let Some(c) = row.last_mut() {
                *c = true;
            }
        }
    }

    for _ in 0..100 {
        for vec in new_grid.iter_mut() {
            vec.fill(false)
        }

        for i in 0..grid.len() {
            for j in 0..grid.len() {
                let new_value = is_corner(grid.len() - 1, i, j)
                    || match neighbor_value(&grid, i, j) {
                        true => match neighbor_on_count(&grid, i, j) {
                            2 | 3 => true,
                            _ => false,
                        },
                        false => neighbor_on_count(&grid, i, j) == 3,
                    };

                if let Some(row) = new_grid.get_mut(j) {
                    if let Some(value) = row.get_mut(i) {
                        *value = new_value;
                    }
                }
            }
        }

        mem::swap(&mut new_grid, &mut grid);
    }

    let on: usize = grid
        .iter()
        .map(|row| row.iter().filter(|c| **c).count())
        .sum();
    on.to_string()
}
